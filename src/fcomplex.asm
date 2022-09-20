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
    
; Multiply four pairs of complex numbers
; fcomplex fcmul_4_asm(fcomplex[4]x(ymm0), fcomplex[4]y(ymm1))
global fcmul_4_asm
fcmul_4_asm:
    vmovsldup ymm2, ymm0 
    vmulps ymm2, ymm1    

    vshufps ymm1, ymm1, 0xb1
    vmovshdup ymm3, ymm0    
    vmulps ymm3, ymm1       

    vaddsubps ymm2, ymm3

    vmovaps ymm0, ymm2  
    ret

global fcabs_pair_asm
; Absolute value of a pair of complex numbers
; float fcabs_pair_asm(fcomplex[2]x(xmm0))
fcabs_pair_asm:
                      ; xmm0 = | x1.im              | x1.re             | x0.im             | x0.re             |
    mulps xmm0, xmm0  ; xmm0 = | x1.im^2            | x1.re^2           | x0.im^2           | x0.re^2           |
    haddps xmm0, xmm0 ; xmm0 = | ...                | ...               | x1.im^2 + x1.re^2 | x0.im^2 + x0.re^2 |
    sqrtps xmm0, xmm0 ; xmm0 = | ...                | ...               | abs(x1)           | abs(x0)           |
    ret

global fcabs_4_asm
; Absolute value of four complex numbers
; float fcabs_pair_asm(fcomplex[4]x(ymm0))
fcabs_4_asm:
    vmulps ymm0, ymm0
    vhaddps ymm0, ymm0
    vsqrtps ymm0, ymm0
    vextractf128 xmm1, ymm0, 0x1
    vshufps xmm0, xmm1, 0x44
    ret