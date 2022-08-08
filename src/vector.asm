section .data
%define FCOMPLEX_LENGTH 8
%define FLOAT_LENGTH 4

section .text
extern fcmul_pair_asm
extern fcmul_4_asm

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
    %define i rcx
    %define sum ymm0

                    xor i, i
                    vxorps sum, sum
                    mov rax, 7
                    cmp rax, length
                    jge .lessThan8
                    sub length, 7

    .iLoop:         ; for (i = 0; i < length - 7; i+=8)
                    vmovups ymm1, YWORD[v1 + i * FLOAT_LENGTH]
                    vmovups ymm2, YWORD[v2 + i * FLOAT_LENGTH]
                    vmulps  ymm1, ymm2
                    vaddps  sum, ymm1

                    add i, 8
                    cmp i, length
                    jl .iLoop
                    add length, 7

    .lessThan8:     cmp i, length ; for (i = i; i < length; i++)
                    je .end

                    vmovd xmm1, DWORD[v1 + i * FLOAT_LENGTH]
                    vmovd xmm2, DWORD[v2 + i * FLOAT_LENGTH]
                    vmulss xmm1, xmm2
                    vaddps sum, ymm1

                    add i, 1
                    jmp .lessThan8

    .end:           vextractf128 xmm1, sum, 0x1
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
    %define i r8
                    xor i, i
                    mov rax, 7
                    cmp rax, length
                    jge %%.lessThan8
                    sub length, 7

    %%.iLoop:         ; for (i = 0; i < length - 7; i+=8)
                    vmovups ymm0, YWORD[v1 + i * FLOAT_LENGTH]
                    vmovups ymm1, YWORD[v2 + i * FLOAT_LENGTH]
                    %1  ymm0, ymm1
                    vmovups YWORD[result + i * FLOAT_LENGTH], ymm0

                    add i, 8
                    cmp i, length
                    jl %%.iLoop
                    add length, 7

    %%.lessThan8:   cmp i, length ; for (i = i; i < length; i++)
                    je %%.end

                    vmovd  xmm0, DWORD[v1 + i * FLOAT_LENGTH]
                    vmovd  xmm1, DWORD[v2 + i * FLOAT_LENGTH]
                    %2 xmm0, xmm1
                    vmovd  DWORD[result + i * FLOAT_LENGTH], xmm0

                    add i, 1
                    jmp %%.lessThan8

    %%.end:           ret
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

                    vmovd  xmm0, DWORD[vec + i * FLOAT_LENGTH]
                    vmulss xmm0, xmm1
                    vmovd  DWORD[result + i * FLOAT_LENGTH], xmm0

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

    .iLoop:         ; for (i = 0; i < length; i+=2)
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
