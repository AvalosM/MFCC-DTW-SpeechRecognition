section .data
extern fcmul_pair_asm
%define FCOMPLEX_LENGTH 8
%define FLOAT_LENGTH 4

section .text

global vectorf_dot_asm
; float vectorf_dot_asm(float *v1(rdi), float *v2(rsi), unsigned int length(rdx))
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
    
    %define sum xmm0
    xorps sum, sum

    .iLoop: ; for (i = 0; i < length; i+=4)
    cmp i, length
    jge .checkLast

    movups xmm1, OWORD[x + i * FLOAT_LENGTH] ; xmm1 = | v1[i+3]                 | v1[i+2]                 | v1[i+1]                 | v1[i]               |
    movups xmm2, OWORD[y + i * FLOAT_LENGTH] ; xmm2 = | v2[i+3]                 | v2[i+2]                 | v2[i+1]                 | v2[i]               |
    mulps xmm1, xmm2                         ; xmm1 = | v2[i+3]                 | v2[i+2]                 | v2[i+1]                 | v2[i]               |
    addps sum, xmm1                          ; xmm0 = | sun + v1[i+3] * v2[i+3] | sum + v1[i+2] * v2[i+2] | sum + v1[i+1] * v2[i+1] | sum + v1[i] * v2[i] |

    add i, 4
    jmp .iLoop

    .checkLast:
    cmp i, rdx
    je .end
    
    movd xmm1, DWORD[x + i * FLOAT_LENGTH] ; xmm1 = | ... | ... | ... | v1[i]               |
    movd xmm2, DWORD[y + i * FLOAT_LENGTH] ; xmm2 = | ... | ... | ... | v2[i]               |
    mulps xmm1, xmm2                       ; xmm1 = | ... | ... | ... | v2[i]               |
    addps sum, xmm1                        ; xmm0 = | ... | ... | ... | sum + v1[i] * v2[i] |

    add i, 1
    jmp .checkLast

    .end:
    haddps xmm0, xmm0
    haddps xmm0, xmm0

    ret

global vectorf_mul_r_asm
; void vectorf_mul_r_asm(float *v1(rdi), float *v2(rsi), unsigned int length(rdx))
vectorf_mul_r_asm:
    ; ----------------
    ; ------Body------
    ; ----------------
    %define v1 rdi
    %define v2 rsi

    %define length r8
    mov length, rdx
    sub length, 3

    %define i rcx
    mov i, 0

    .iLoop: ; for (i = 0; i < length; i+=4)
    cmp i, length
    jge .checkLast

    movups xmm0, OWORD[v1 + i * FLOAT_LENGTH] ; xmm0 = | v1[i+3]           | v1[i+2]           | v1[i+1]           | v1[i]         |
    movups xmm1, OWORD[v2 + i * FLOAT_LENGTH] ; xmm1 = | v2[i+3]           | v2[i+2]           | v2[i+1]           | v2[i]         |
    mulps xmm0, xmm1                          ; xmm0 = | v1[i+3] * v2[i+3] | v1[i+2] * v2[i+2] | v1[i+1] * v2[i+1] | v1[i] * v2[i] |
    movups OWORD[v1 + i * FLOAT_LENGTH], xmm0

    add i, 4
    jmp .iLoop

    .checkLast:
    cmp i, rdx
    je .end

    movd xmm0, DWORD[v1 + i * FLOAT_LENGTH] ; xmm0 = | ... | ... | ... | v1[i]         |
    movd xmm1, DWORD[v2 + i * FLOAT_LENGTH] ; xmm1 = | ... | ... | ... | v2[i]         |
    mulps xmm0, xmm1                        ; xmm0 = | ... | ... | ... | v1[i] * v2[i] |
    movd DWORD[v1 + i * FLOAT_LENGTH], xmm0

    add i, 1
    jmp .checkLast

   .end:
    ret 

global vectorf_smul_r_asm
; void vectorf_mul_r_asm(float *vec(rdi), scalar(xmm0), unsigned int length(rsi))
vectorf_smul_r_asm:
    ; ----------------
    ; ------Body------
    ; ----------------
    %define vec rdi
    %define scalar xmm0
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
    jmp .iLoop

    .checkLast:
    cmp i, rsi
    je .end

    movd xmm1, DWORD[vec + i * FLOAT_LENGTH] ; xmm1 = | 0 | 0 | 0 | vec[i]          |
    mulps xmm1, scalar                       ; xmm1 = | 0 | 0 | 0 | vec[i] * scalar |
    movd DWORD[vec + i * FLOAT_LENGTH], xmm1

    add i, 1
    jmp .checkLast

    .end:
    ret