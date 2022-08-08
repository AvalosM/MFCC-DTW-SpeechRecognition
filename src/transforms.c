#include "transforms.h"
#include "vector.h"
#include <string.h>

fcomplex *twiddle = NULL;

void twiddle_init(unsigned int size)
{
    static unsigned int init_size = 0;
    
    if (init_size != size) {
        free(twiddle);
        twiddle = (fcomplex*)malloc(size * sizeof(fcomplex));
        if (!twiddle) {
        exit(-1);
        }

        for (unsigned int q = 0; (unsigned int)1 << q <= size; q++) {
            unsigned int L = 1 << q;  /* L = 2^q; */
            unsigned int Ls = L >> 1; /* Ls = L / 2 */
            for (unsigned int j = 0; j < Ls; j++) {
                twiddle[Ls - 1 + j].real = cosf(2 * PI * j / L);
                twiddle[Ls - 1 + j].imag = -sinf(2 * PI * j / L);
            }
        }
    }
}

void fftstockham(fcomplex *x, fcomplex *workspace, unsigned int N)
{
    fcomplex *y_ini = workspace;
    /* For q=1 to log_2(N) */
    for (unsigned int L = 2; L <= N; L <<= 1) {
        
        /* Swap pointers */
        fcomplex *tmp = x;
        x = workspace;
        workspace = tmp;

        unsigned int Ls = L >> 1;
        unsigned int r = N / L;
        unsigned int rs = N / Ls;

        for (unsigned int j = 0; j < Ls; j++) {
            /* Twiddle factor */
            fcomplex w = twiddle[Ls - 1 + j];
            
            for (unsigned int k = 0; k < r; k++) {
                fcomplex t = fcmul(w, workspace[j * rs + k + r]);
                x[(j * r) + k] = fcadd(workspace[(j * rs) + k], t);
                x[((j + Ls) * r) + k] = fcsub(workspace[(j * rs) + k], t);
            }
        }
    }
    /* If log_2(N) is odd, result values are stored on additional workspace */
    if (workspace != y_ini) {
        for (unsigned int i = 0; i < N; i++) {
            workspace[i] = x[i];
        }
    }
}

void fftstockham_avx(fcomplex *x, fcomplex *y, unsigned int n)
{
    fcomplex *y_ini = y;
    /* Allocate memory for twiddle vector */
    fcomplex *u = malloc((n >> 1) * sizeof(fcomplex));

    /* For q=1 to log_2(n) */
    for (unsigned int q = 1; (unsigned int)1 << q <= n; q++) {
        /* Swap pointers */
        fcomplex *temp = x;
        x = y;
        y = temp;

        unsigned int L = 1 << q;  /* L = 2^q; */
        unsigned int Ls = L >> 1; /* Ls = L / 2 */
        unsigned int r = n >> q;  /* r = n / L */

        for (unsigned int k = 0; k < r; k++) {
            vectorfc_mul_asm(twiddle + Ls - 1, y + ((k + r) * Ls), u, Ls);
            vectorfc_add_asm(y + k * Ls, u, x + k * L, Ls);
            vectorfc_sub_asm(y + k * Ls, u, x + (k * L + Ls), Ls);
        }
    }
    /* If log_2(N) is odd, result values are stored on additional workspace */
    if (y != y_ini) {
        for (unsigned int i = 0; i < n; i++) {
            y[i] = x[i];
        }
    }
    free(u);
}

void fft(fcomplex *x, fcomplex *workspace, unsigned int N)
{
    #ifdef __FFT_AVX__
        fftstockham_avx(x, workspace, N);
    #else
        fftstockham(x, workspace, N);
    #endif
}

/* Naive Discrete cosine transform II implementation */
/* https://en.wikipedia.org/wiki/Discrete_cosine_transform */
void dct_naive(float *signal, unsigned int length)
{
    float *workspace = malloc(length * sizeof(float));
    for (unsigned int k = 0; k < length; k++) {
        float sum = 0;
        for (unsigned int n = 0; n < length; n++) {
            sum += signal[n] * cosf((n + 0.5) * k * PI / length);
        }
        workspace[k] = sum;
    }
    for (unsigned int i = 0; i < length; i++) {
        signal[i] = workspace[i];
    }
    free(workspace);
}

void fct(float *signal, unsigned int length)
{
    dct_naive(signal, length);
}