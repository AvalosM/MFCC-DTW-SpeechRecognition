section .data
section .text

global fcmul_pair_asm
; Multiply two pairs of complex numbers
; fcomplex fcmul_pair(fcomplex[2]x(xmm0), fcomplex[2]y(xmm1))
; Reference: Intel® 64 and IA-32 Architectures Optimization Reference Manual
fcmul_pair_asm:
                        ; xmm1 = | y1.im         | y1.re         | y0.im         | y0.re         |
    movsldup xmm2, xmm0 ; xmm2 = | x1.re         | x1.re         | x0.re         | x0.re         |
    mulps xmm2, xmm1    ; xmm2 = | x1.re * y1.re | x1.re * y1.re | x0.re * y0.re | x0.re * y0.re |

    shufps xmm1, xmm1, 0xb1 ; xmm1 = | y1.re         | y1.im         | y0.re         | y0.im         |
    movshdup xmm3, xmm0     ; xmm3 = | x1.im         | x1.im         | x0.im         | x0.im         |
    mulps xmm3, xmm1        ; xmm3 = | x1.im * y1.re | x1.im * y1.im | x0.im * y0.re | x0.im * y0.im |

    addsubps xmm2, xmm3 ; xmm2 = | x1 * y1 | x0 * y0 |

    movaps xmm0, xmm2  
    ret