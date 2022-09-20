#include "transforms.h"
#include "vector.h"
#include <string.h>

fcomplex *twiddle_factors(unsigned int fft_size)
{
    fcomplex *twiddle = malloc(fft_size * sizeof(fcomplex));
    if (!twiddle) exit(-1);

    for (unsigned int q = 0; (unsigned int)1 << q <= fft_size; q++) {
        unsigned int L = 1 << q;  /* L = 2^q; */
        unsigned int Ls = L >> 1; /* Ls = L / 2 */
        for (unsigned int j = 0; j < Ls; j++) {
            twiddle[Ls - 1 + j].real = cosf(2 * PI * j / L);
            twiddle[Ls - 1 + j].imag = -sinf(2 * PI * j / L);
        }
    }

    return twiddle;
}

void fft(fcomplex *x, fcomplex *y, fcomplex *twiddle, unsigned int n)
{
    fcomplex *y_ini = y;

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
            vectorfc_mul(twiddle + Ls - 1, y + ((k + r) * Ls), y + ((k + r) * Ls), Ls);
            vectorfc_add(y + k * Ls, y + ((k + r) * Ls), x + k * L, Ls);
            vectorfc_sub(y + k * Ls, y + ((k + r) * Ls), x + (k * L + Ls), Ls);
        }
    }
    /* If log_2(N) is odd, result values are stored on additional workspace */
    if (y != y_ini) {
        for (unsigned int i = 0; i < n; i++) {
            y[i] = x[i];
        }
    }
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