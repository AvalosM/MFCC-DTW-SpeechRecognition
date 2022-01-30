section .data
extern twiddle
%define TWIDDLE_SIZE 1024
%define FCOMPLEX_LENGTH 8
%define FCOMPLEX_ITEM_OFFSET 4
section .text
global fftstockham_asm

; Multiply two pairs of complex numbers
; float complex cmul_pair(fcomplex x(xmm0), fcomplex y(xmm1))
; Reference: 
cmul_pair:
    ;        127                             0
    ; xmm2 = | y1.re | y1.re | y0.re | y0.re |
    movsldup xmm2, xmm0

    ; xmm3 = | x1.im | x1.re | x0.im | x0.re |
    ; movaps xmm3, xmm1

    ; xmm2 = | x1.im * y1.re | x1.re * y1.re | x0.im * y0.re | x0.re * y0.re |
    mulps xmm2, xmm1

    ; xmm0 = | x1.re | x1.im | x0.re | x0.im |
    shufps xmm1, xmm1, 0xb1

    ; xmm3 = | y1.im | y1.im | y0.im | y0.im |
    movshdup xmm3, xmm0

    ; xmm3 = | x1.im * y1.im | x1.re * y1.im | x0.im * y0.im | x0.re * y0.im |
    mulps xmm3, xmm1

    ; xmm2 = | x1 * y1 | x0 * y0 |
    addsubps xmm2, xmm3

    movaps xmm0, xmm2 

    ret


; void stockhamfft_asm(complex *x(rdi), complex *y(rsi), unsigned int N(rdx))             
fftstockham_asm:

    ; ----------------
    ; ----Prologue----
    ; ----------------
    push rbx
    push r12
    push r13
    push r14
    push r15

    ; ----------------
    ; ------Body------
    ; ----------------
    %define x rdi
    %define y rsi
    %define N r8
    mov N, rdx

    %define y_ini r9
    mov y_ini, y

    %define L r10
    mov L, 2
    .Lloop: ; for (L = 2; L <= N; L <<= 1)

    ; Swap pointers
    xchg x, y
    
    %define Ls r11
    mov Ls, L
    shr Ls, 1

    %define r r12
    mov rax, N
    xor rdx, rdx
    div L
    mov r, rax

    %define rs r13
    mov rax, N
    div Ls
    mov rs, rax

    %define j r14
    mov j, 0
    .jloop: ; for (j = 0; j < Ls; j++)

    ; rax = (j * TWIDDLE_SIZE / LS) * sizeof(fcomplex)
    mov rax, TWIDDLE_SIZE
    mul j
    div Ls
    mov rbx, FCOMPLEX_LENGTH
    mul rbx

    %define w xmm5
    ; xmm3 = | twiddle[rax] | twiddle[rax] |
    movlps w, [twiddle + rax]
    movhps w, [twiddle + rax]

    %define k r15
    mov k, 0
    .kloop: ; for (k = 0; k < r; k+=2)

    ; rax = (j * rs + k + r) * sizeof(fcomplex)
    mov rax, j
    mul rs
    add rax, k
    add rax, r
    ; mov rbx, FCOMPLEX_LENGTH
    mul rbx

    %define t xmm0
    ; xmm0 = | y[rax + 1] | y[rax] |
    movlps t, [y + rax]
    movhps t, [y + rax + FCOMPLEX_LENGTH]
    
    movaps xmm1, w
    ; xmm0 = |w * y[rax + 1] | w * y[rax] |
    call cmul_pair

    ; rax = (j * rs + k) * sizeof(fcomplex)
    mov rax, j
    mul rs
    add rax, k
    ; mov rbx, FCOMPLEX_LENGTH
    mul rbx

    ; xmm2 = | y[rax + 1] | y[rax] |
    movups xmm2, [y + rax]

    ; xmm1 = | y[rax + 1] + t | y[rax] + t |
    movups xmm1, xmm2
    addps xmm1, t

    ; rax = (j * r + k)
    mov rax, j
    mul r
    add rax, k
    ; mov rbx, FCOMPLEX_LENGTH
    mul rbx

    ; x[rax] = y[rax] + t
    ; x[rax + 1] = y[rax + 1] + t
    movups [x + rax], xmm1
    
    ; xmm1 = | y[rax + 1] - t | y[rax] - t |
    movaps xmm1, xmm2
    subps xmm1, t

    ; rax = ((j + Ls) * r + k) * sizeof(fcomplex)
    mov rax, j
    add rax, Ls
    mul r
    add rax, k
    mul rbx
    
    ; x[rax] = y[rax] - t
    ; x[rax + 1] = y[rax + 1] - t
    movups[x + rax], xmm1

    inc k
    cmp k, r
    jl .kloop

    inc j
    cmp j, Ls
    jl .jloop

    shl L, 1
    cmp L, N
    jle .Lloop
    jmp .end
    
    .end:
    ; ----------------
    ; ----Epilogue----
    ; ----------------
    pop rbx
    pop r12
    pop r13
    pop r14
    pop r15

    ret


