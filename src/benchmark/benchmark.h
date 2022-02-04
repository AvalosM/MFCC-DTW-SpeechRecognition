#include <time.h>

#define CLK_NOW(clk)                \
    __asm__ __volatile__(           \
        "xor %%eax, %%eax\n\t"      \
        "cpuid\n\t"                 \
        "rdtsc\n\t"                 \
        "shl $32, %%rdx\n\t"        \
        "or %%rdx, %%rax\n\t"       \
        : "=r" (clk)                \
    )


/* https://www.gnu.org/software/libc/manual/html_node/CPU-Time.html */
#define CPU_TIME(start, end)                    \
    ((double) (end - start)) / CLOCKS_PER_SEC;

/* Bemchmark functions */
void fft_benchmark();
