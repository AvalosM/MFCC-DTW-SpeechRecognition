section .data
extern twiddle
extern cmul_pair
%define TWIDDLE_SIZE 1024
%define FCOMPLEX_LENGTH 8

section .text
global fftstockham_asm

; void stockhamfft_asm(fcomplex *x(rdi), fcomplex *y(rsi), unsigned int N(rdx))             
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

    xchg x, y ; Swap pointers
    
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

    mov rax, TWIDDLE_SIZE
    mul j
    div Ls
    shl rax, 3 ; rax = (j * TWIDDLE_SIZE / LS) * sizeof(fcomplex)

    %define w xmm5
    movlps w, QWORD [twiddle + rax]
    movhps w, QWORD [twiddle + rax]  ; xmm3 = | twiddle[rax] | twiddle[rax] |
   
    %define k r15
    mov k, 0

    cmp r, 1 ; if (r == 1) there is only one pair to multiply
    jne .kloop

    mov rax, j
    shl rax, 1
    inc rax
    shl rax, 3 ; rax = (j * rs + k + r) * sizeof(fcomplex) = (j * 2 + 1) * sizeof(fcomplex)

    %define t xmm0   
    movlps t, QWORD [y + rax]  ; xmm0 = | ... | y[rax]     |
    movaps xmm1, w             ; xmm1 = | ... | w          | 
    call cmul_pair             ; xmm1 = | ... | w * y[rax] |

    sub rax, FCOMPLEX_LENGTH ; rax = (j * rs + k) * sizeof(fcomplex) = (j * 2) * sizeof(fcomplex)

    movlps xmm2, QWORD [y + rax] ; xmm2 = | ... | y[rax]     |
    movaps xmm1, xmm2            ; xmm1 = | ... | y[rax]     |
    addps xmm1, t                ; xmm1 = | ... | y[rax] + t |

    shr rax, 1  ; rax* = (j * r + k) * sizeof(fcomplex) = j * sizeof(fcomplex)

    movlps QWORD [x + rax], xmm1 ; x[rax*] = y[rax] + t

    subps xmm2, t                ; xmm2 = | ... | y[rax] - t |

    mov rax, j
    add rax, Ls
    shl rax, 3  ; rax* = ((j + Ls) * r + k) * sizeof(fcomplex) = (j + Ls) * sizeof(fcomplex)
    
    movlps QWORD [x + rax], xmm2 ; x[rax*] = y[rax] - t

    jmp .kend

    .kloop: ; for (k = 0; k < r; k+=2)

    mov rax, j
    mul rs
    add rax, k
    add rax, r
    shl rax, 3 ; rax = (j * rs + k + r) * sizeof(fcomplex)

    movups t, OWORD [y + rax] ; xmm0 = | y[rax + 1]    | y[rax]     |  
    movaps xmm1, w            ; xmm1 = | w             | w          | 
    call cmul_pair            ; xmm0 = |w * y[rax + 1] | w * y[rax] |

    mov rax, j
    mul rs
    add rax, k
    shl rax, 3 ; rax = (j * rs + k) * sizeof(fcomplex)

    movups xmm2, OWORD [y + rax] ; xmm2 = | y[rax + 1]     | y[rax]     |
    movaps xmm1, xmm2            ; xmm1 = | y[rax + 1]     | y[rax]     |
    addps xmm1, t                ; xmm1 = | y[rax + 1] + t | y[rax] + t |

    mov rax, j
    mul r
    add rax, k
    shl rax, 3 ; rax* = (j * r + k) * sizeof(fcomplex)

    movups OWORD [x + rax], xmm1     ; x[rax*]     = y[rax] + t
                                     ; x[rax* + 1] = y[rax + 1] + t

    subps xmm2, t ; xmm2 = | y[rax + 1] - t | y[rax] - t |

    mov rax, j
    add rax, Ls
    mul r
    add rax, k
    shl rax, 3 ; rax* = ((j + Ls) * r + k) * sizeof(fcomplex)
    
    movups OWORD [x + rax], xmm2 ; x[rax*]     = y[rax] - t
                                 ; x[rax* + 1] = y[rax + 1] - t
                                 
    add k, 2
    cmp k, r
    jl .kloop

    .kend:
    inc j
    cmp j, Ls
    jl .jloop

    shl L, 1
    cmp L, N
    jle .Lloop
    
    cmp y_ini, y ; If log_2(N) is odd, result values are stored on additional workspace y
    je .end

    mov rcx, 0
    .iloop: ; for (unsigned int i = 0; i < N; i++)
    cmp rcx, N
    je .end

    mov rax, QWORD [x + rcx * FCOMPLEX_LENGTH]
    mov QWORD[y + rcx * FCOMPLEX_LENGTH], rax

    inc rcx
    jmp .iloop

    ; ----------------
    ; ----Epilogue----
    ; ----------------
    .end:
    pop rbx
    pop r12
    pop r13
    pop r14
    pop r15

    ret


