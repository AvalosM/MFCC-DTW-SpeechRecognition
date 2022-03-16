section .data
%define FCOMPLEX_LENGTH 8
%define FLOAT_LENGTH 4
section .text

global fcmul_pair
; Multiply two pairs of complex numbers
; fcomplex fcmul_pair(fcomplex[2]x(xmm0), fcomplex[2]y(xmm1))
; Reference: Intel® 64 and IA-32 Architectures Optimization Reference Manual
fcmul_pair:
                        ; xmm1 = | y1.im         | y1.re         | y0.im         | y0.re         |
    movsldup xmm2, xmm0 ; xmm2 = | x1.re         | x1.re         | x0.re         | x0.re         |
    mulps xmm2, xmm1    ; xmm2 = | x1.re * y1.re | x1.re * y1.re | x0.re * y0.re | x0.re * y0.re |

    shufps xmm1, xmm1, 0xb1 ; xmm1 = | y1.re         | y1.im         | y0.re         | y0.im         |
    movshdup xmm3, xmm0     ; xmm3 = | x1.im         | x1.im         | x0.im         | x0.im         |
    mulps xmm3, xmm1        ; xmm3 = | x1.im * y1.re | x1.im * y1.im | x0.im * y0.re | x0.im * y0.im |

    addsubps xmm2, xmm3 ; xmm2 = | x1 * y1 | x0 * y0 |

    movaps xmm0, xmm2  
    ret

global fcfmul_pair
; Multiply a pair of complex numbers by a pair of floats
; fcomplex fcfmul_pair(fcomplex[2]x(xmm0), float[2]y(xmm1))
fcfmul_pair:
    shufps xmm1, xmm1, 0x50  ; xmm1 = | y1    | y1    | y0    | y0    |
    mulps xmm0, xmm1         ; xmm0 = | x1 * y1       | x0 * y0       |
    
    ret

global fcfdot_asm
; fcomplex and float vector dot product
; fcomplex fcfdot_asm(fcomplex *x(rdi), float *y(rsi), unsigned int length(rdx))
fcfdot_asm:
    ; ----------------
    ; ------Body------
    ; ----------------
    %define x rdi
    %define y rsi

    %define sum xmm2
    xorps sum, sum

    %define i rcx
    mov i, 0
    %define length r8
    mov length, rdx
    dec length

    .iLoop: ; for (i = 0; i < length -1; i++)
    cmp i, length
    jge .checkLast

    movups xmm0, OWORD [x + i * FCOMPLEX_LENGTH] ; xmm0 = | x[i + 1]  | x[i]          |
    movlps xmm1, QWORD [y + i * FLOAT_LENGTH]    ; xmm1 = | ... | ... | y[i+1] | y[i] |

    call fcfmul_pair        ; xmm0 = | x[i + 1] * y[i + 1] | x[i] * y[i]         |
    shufps xmm0, xmm0, 0xd8 ; xmm0 = | mul2.im | mul1.im   | mul2.re | mul1.re   |
    haddps xmm0, xmm0       ; xmm0 = | ...                 | mul1 + mul2         |
    addps sum, xmm0         ; xmm4 = | ...                 | sum + (mul1 + mul2) |

    add i, 2
    jmp .iLoop

    .checkLast: ; If length is odd, there is one multiplication remaining
    jg .end
    
    movlps xmm0, QWORD [x + i * FCOMPLEX_LENGTH] ; xmm0 = | ...       | x[i]        |
    movd xmm1, DWORD [y + i * FLOAT_LENGTH]      ; xmm1 = | ... | ... | ... | y[i]  |
    call fcfmul_pair                             ; xmm0 = | ... | x[i] * y[i]       |
    addps sum, xmm0                              ; xmm4 = | ... | sum + x[i] * y[i] |

    .end:
    movaps xmm0, sum
    ; ----------------
    ; ----Epilogue----
    ; ----------------
    ret