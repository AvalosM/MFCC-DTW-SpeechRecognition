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
    ; ----------------
    ; ----Epilogue----
    ; ----------------
    ret