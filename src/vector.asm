section .data
extern fcmul_pair_asm
%define FCOMPLEX_LENGTH 8
%define FLOAT_LENGTH 4

section .text

global vectorfc_dot_asm
; fcomplex vector dot product
; fcomplex vectorfc_dot_asm(fcomplex *x(rdi), fcomplex *y(rsi), unsigned int length(rdx))
vectorfc_dot_asm:
    ; ----------------
    ; ------Body------
    ; ----------------
    %define x rdi
    %define y rsi

    %define sum xmm4
    xorps sum, sum

    %define i rcx
    mov i, 0
    %define length r8
    mov length, rdx
    dec length

    .iLoop: ; for (i = 0; i < length -1; i+=2)
    cmp i, length
    jge .checkLast

    movups xmm0, OWORD [x + i * FCOMPLEX_LENGTH] ; xmm0 = | x[i + 1] | x[i] |
    movups xmm1, OWORD [y + i * FCOMPLEX_LENGTH] ; xmm1 = | y[i + 1] | y[i] |

    call fcmul_pair_asm     ; xmm0 = | x[i + 1] * y[i + 1] | x[i] * y[i]         |
    shufps xmm0, xmm0, 0xd8 ; xmm0 = | mul2.im | mul1.im   | mul2.re | mul1.re   |
    haddps xmm0, xmm0       ; xmm0 = | ...                 | mul1 + mul2         |
    addps sum, xmm0         ; xmm4 = | ...                 | sum + (mul1 + mul2) |

    add i, 2
    jmp .iLoop

    .checkLast: ; If length is odd, there is one multiplication remaining
    jg .end
    
    movlps xmm0, QWORD [x + i * FCOMPLEX_LENGTH] ; xmm0 = | ... | x[i]              |
    movlps xmm1, QWORD [y + i * FCOMPLEX_LENGTH] ; xmm1 = | ... | y[i]              |
    call fcmul_pair_asm                          ; xmm0 = | ... | x[i] * y[i]       |
    addps sum, xmm0                              ; xmm4 = | ... | sum + x[i] * y[i] |

    .end:
    movaps xmm0, sum

    ret

global vectorf_dot_asm
;
;
vectorf_dot_asm:
    ; ----------------
    ; ------Body------
    ; ----------------
    %define x rdi
    %define y rsi

    %define length r8
    mov length, rdx
    sub length, 3

    %define i rcx
    mov i, 0
    
    %define sum, xmm0
    xorps sum, sum

    .iLoop: ; for (i = 0; i < length; i+=4)
    cmp i, length
    jge .checkLast

    movups xmm1, OWORD[x + i * FLOAT_LENGTH]
    movups xmm2, OWORD[y + i * FLOAT_LENGTH] 
    mulps xmm1, xmm2
    addps sum, xmm1

    add i, 4
    jmp .iLoop

    .checkLast:
    cmp i, length
    je .end
    
    movd xmm1, DWORD[x + i * FLOAT_LENGTH]
    movd xmm2, DWORD[y + i * FLOAT_LENGTH]
    mulps xmm1, xmm2
    addps sum, xmm1

    add i, 1
    jmp .checkLast

    .end:
    haddps xmm0, xmm0
    haddps xmm0, xmm0

    ret

global vectorf_smul_r_asm
; void vectorf_mul_r_asm(float *vec(rdi), scalar(xmm0), unsigned int length(rsi))
vectorf_smul_r_asm:
    ; ----------------
    ; ------Body------
    ; ----------------
    %define vec rdi
    %define scalar, xmm0
                               ; xmm0 = | ...    | ...    | ...    | scalar |    
    shufps scalar, scalar, 0x0 ; xmm0 = | scalar | scalar | scalar | scalar |
    
    %define length r8
    mov length, rsi
    sub length, 3

    %define i rcx
    mov i, 0

    .iLoop: ; for (i = 0; i < length; i+=4)
    cmp i, length
    jge .checkLast

    movups xmm1, OWORD[vec + i * FLOAT_LENGTH] ; xmm1 = | vec[i+3]         | vec[i+2]          | vec[i+1]          | vec[i]          |
    mulps xmm1, scalar                         ; xmm1 = | vec[i+3] * scalar| vec[i+2] * scalar | vec[i+1] * scalar | vec[i] * scalar |
    movups OWORD[vec + i * FLOAT_LENGTH], xmm1

    add i, 4
    jmp .iloop

    .checkLast:
    cmp i, rdx
    je .end

    movd xmm1, DWORD[vec + i * FLOAT_LENGTH] ; xmm1 = | 0 | 0 | 0 | vec[i]          |
    mulps xmm1, scalar                       ; xmm1 = | 0 | 0 | 0 | vec[i] * scalar |
    movups DWORD[vec + i * FLOAT_LENGTH], xmm1

    add i, 1
    jmp .checkLast

    .end:
    ret

