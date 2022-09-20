section .data
%define FCOMPLEX_LENGTH 8
%define FLOAT_LENGTH 4

section .text
extern fcmul_pair_asm
extern fcmul_4_asm
extern fcabs_pair_asm
extern fcabs_4_asm

;-------------------------------------------------------------------------------
; Float vector dot product
;
; C signature:
; void vectorf_mul_asm(float *v1(rdi), float *v2(rsi), unsigned int length(rdx))
;-------------------------------------------------------------------------------
global vectorf_dot_asm
vectorf_dot_asm:
    %define v1 rdi
    %define v2 rsi
    %define length rdx

                                vxorps ymm0, ymm0

                                jmp .eight_loop_condition
    .eight_loop:
                                vmovups ymm1, YWORD[v1]
                                vmovups ymm2, YWORD[v2]
                                vmulps  ymm1, ymm2
                                vaddps  ymm0, ymm1

                                add v1, 8 * FLOAT_LENGTH
                                add v2, 8 * FLOAT_LENGTH
                                sub length, 8

    .eight_loop_condition:      cmp length, 8
                                jae .eight_loop
                                
                                jmp .one_loop_condition
    .one_loop:
                                vmovss xmm1, DWORD[v1]
                                vmovss xmm2, DWORD[v2]
                                vmulss xmm1, xmm2
                                vaddps ymm0, ymm1

                                add v1, FLOAT_LENGTH
                                add v2, FLOAT_LENGTH
                                dec length

    .one_loop_condition:        cmp length, 1
                                jae .one_loop

                                vextractf128 xmm1, ymm0, 0x1
                                vaddps       xmm0, xmm1, xmm0
                                vmovshdup    xmm1, xmm0
                                vaddps       xmm0, xmm1
                                vmovhlps     xmm1, xmm0
                                vaddss       xmm0, xmm1

                                ret

; TODO name this :|
%macro abstraction 2
    %define v1 rdi
    %define v2 rsi
    %define result rdx
    %define length rcx

                                jmp %%.eight_loop_condition
    %%.eight_loop:              ; for (i = 0; i < length - 7; i+=8)
                                vmovups ymm0, YWORD[v1]
                                vmovups ymm1, YWORD[v2]
                                %1  ymm0, ymm1
                                vmovups YWORD[result], ymm0

                                add v1,     8 * FLOAT_LENGTH
                                add v2,     8 * FLOAT_LENGTH
                                add result, 8 * FLOAT_LENGTH 
                                sub length, 8

    %%.eight_loop_condition:    cmp length, 8
                                jae %%.eight_loop

                                jmp %%.one_loop_condition
    %%.one_loop:
                                vmovss  xmm0, DWORD[v1]
                                vmovss  xmm1, DWORD[v2]
                                %2 xmm0, xmm1
                                vmovss  DWORD[result], xmm0

                                add v1,     FLOAT_LENGTH
                                add v2,     FLOAT_LENGTH
                                add result, FLOAT_LENGTH
                                dec length

    %%.one_loop_condition:      cmp length, 1
                                jae %%.one_loop

                                ret
%endmacro

;-------------------------------------------------------------------------------
; Float vector entrywise multiplication
;
; C signature:
; void vectorf_mul_asm(float *v1(rdi), float *v2(rsi), float* result(rdx), unsigned int length(rcx))
;-------------------------------------------------------------------------------
global vectorf_mul_asm
vectorf_mul_asm:
    abstraction vmulps, vmulss

;-------------------------------------------------------------------------------
; Float vector addition
;
; C signature:
; void vectorf_add_asm(float *v1(rdi), float *v2(rsi), float* result(rdx), unsigned int length(rcx))
;-------------------------------------------------------------------------------
global vectorf_add_asm
vectorf_add_asm:
    abstraction vaddps, vaddss

;-------------------------------------------------------------------------------
; Float vector subtraction
;
; C signature:
; void vectorf_sub_asm(float *v1(rdi), float *v2(rsi), float* result(rdx), unsigned int length(rcx))
;-------------------------------------------------------------------------------
global vectorf_sub_asm
vectorf_sub_asm:
    abstraction vsubps, vsubss

;-------------------------------------------------------------------------------
; Float vector multiplication by scalar
;
; C signature:
; void vectorf_smul_asm(float *vec(rdi), scalar(xmm0), float *result(rsi), unsigned int length(rdx))
;-------------------------------------------------------------------------------
global vectorf_smul_asm
vectorf_smul_asm:
    %define vec rdi
    %define result rsi
    %define length rdx
    %define i rcx
                    vbroadcastss ymm1, xmm0
                    xor i, i
                    mov rax, 7
                    cmp rax, length
                    jge .lessThan8
                    sub length, 7
    
    .iLoop:         ; for (i = 0; i < length - 7; i+=8)
                    vmovups ymm0, YWORD[vec + i * FLOAT_LENGTH]
                    vmulps  ymm0, ymm1
                    vmovups YWORD[result + i * FLOAT_LENGTH], ymm0

                    add i, 8
                    cmp i, length
                    jl .iLoop
                    add length, 7

    .lessThan8:     cmp i, length ; for (i = i; i < length; i++)
                    je .end

                    vmovss  xmm0, DWORD[vec + i * FLOAT_LENGTH]
                    vmulss xmm0, xmm1
                    vmovss  DWORD[result + i * FLOAT_LENGTH], xmm0

                    add i, 1
                    jmp .lessThan8          

    .end:           ret

; ------------------------------------------------------------------------------
; ------------------------------------------------------------------------------
; Fcomplex vector operations
; ------------------------------------------------------------------------------
; ------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
; Fcomplex vector entrywise multiplication
;
; C signature:
; void vectorfc_mul_asm(fcomplex *v1(rdi), fcomplex *v2(rsi), fcomplex *result(rdx), unsigned int length(rcx))
;-------------------------------------------------------------------------------
global vectorfc_mul_asm
vectorfc_mul_asm:
    %define v1 rdi
    %define v2 rsi
    %define result rdx
    %define length rcx
    %define i r8
                    xor i, i
                    mov rax, 3
                    cmp rax, length
                    jge .lessThan4
                    sub length, 3

    .iLoop:         ; for (i = 0; i < length; i+=4)
                    vmovups ymm0, YWORD[v1 + i * FCOMPLEX_LENGTH]
                    vmovups ymm1, YWORD[v2 + i * FCOMPLEX_LENGTH]
                    call fcmul_4_asm wrt ..plt
                    vmovups YWORD[result + i *FCOMPLEX_LENGTH], ymm0

                    add i, 4
                    cmp i, length
                    jl .iLoop
                    add length, 3

    .lessThan4:     cmp i, length
                    jge .end

                    vmovlps xmm0, QWORD[v1 + i * FCOMPLEX_LENGTH]
                    vmovlps xmm1, QWORD[v2 + i * FCOMPLEX_LENGTH]
                    call fcmul_pair_asm wrt ..plt
                    vmovlps QWORD[result + i * FCOMPLEX_LENGTH], xmm0

                    add i, 1
                    jmp .lessThan4

    .end:           ret

;-------------------------------------------------------------------------------
; Fcomplex vector addition
;
; C signature:
; void vectorfc_add_asm(fcomplex *v1(rdi), fcomplex *v2(rsi), fcomplex *result(rdx), unsigned int length(rcx))
;-------------------------------------------------------------------------------
global vectorfc_add_asm
vectorfc_add_asm:
    shl rcx, 1
    call vectorf_add_asm
    ret

;-------------------------------------------------------------------------------
; Fcomplex vector subtraction
;
; C signature:
; void vectorfc_sub_asm(fcomplex *v1(rdi), fcomplex *v2(rsi), fcomplex *result(rdx), unsigned int length(rcx))
;-------------------------------------------------------------------------------
global vectorfc_sub_asm
vectorfc_sub_asm:
    shl rcx, 1
    call vectorf_sub_asm
    ret

;-------------------------------------------------------------------------------
; Float vector to fcomplex vector
; C signature:
; void vectorf_to_fc_asm(float *vec(rdi), fcomplex *result(rsi), unsigned int length(rdx));
;-------------------------------------------------------------------------------
global vectorf_to_fc_asm
vectorf_to_fc_asm:
    %define vec rdi
    %define result rsi
    %define length rdx
    %define rcx i
                    xor i, i
                    mov rax, 3
                    cmp rax, length
                    jge .lessThan4
                    sub length, 3
                    pxor xmm0, xmm0

    .iLoop:         ; for (i = 0; i < length; i+=4)
                    movaps xmm1, OWORD[vec + i * FLOAT_LENGTH]
                    movaps xmm2, xmm1
                    unpcklps xmm1, xmm0
                    unpckhps xmm2, xmm0
                    movups OWORD[result + i * FCOMPLEX_LENGTH], xmm1
                    movups OWORD[result + i * FCOMPLEX_LENGTH + 2 * FCOMPLEX_LENGTH], xmm2

                    add i, 4
                    cmp i, length
                    jl .iLoop
                    add length, 3

    .lessThan4:     cmp i, length
                    jge .end

                    vmovss xmm1, DWORD[vec + i * FLOAT_LENGTH]
                    vmovlps QWORD[result + i * FCOMPLEX_LENGTH], xmm1

                    add i, 1
                    jmp .lessThan4                  

    .end:           ret


;-------------------------------------------------------------------------------
; Float complex vector entrywise absolute value
; C signature:
; void vectorfc_abs_asm(fcomplex *vec(rdi), float *result(rsi), unsigned int length(rdx))
;--------------------------------------------------------------------------------
global vectorfc_abs_asm
vectorfc_abs_asm:
    %define vec rdi
    %define result rsi
    %define length rdx

                                jmp .four_loop_condition
    .four_loop:
                                vmovups ymm0, YWORD[vec]
                                call fcabs_4_asm wrt ..plt
                                vmovups OWORD[result], xmm0

                                add vec,    4 * FCOMPLEX_LENGTH
                                add result, 4 * FLOAT_LENGTH
                                sub length, 4

    .four_loop_condition:       cmp length, 4
                                jae .four_loop

                                jmp .one_loop_condition
    .one_loop:
                                vmovlps xmm0, QWORD[vec]
                                call fcabs_pair_asm wrt ..plt
                                vmovss DWORD[result], xmm0

                                add vec,    FCOMPLEX_LENGTH
                                add result, FLOAT_LENGTH
                                dec length

    .one_loop_condition:        cmp length, 1
                                jae .one_loop
                                
                                ret