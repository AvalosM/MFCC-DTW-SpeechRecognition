#include "transforms.h"

fcomplex twiddle[TWIDDLE_TABLE_SIZE];

void twiddle_init()
{
    for (unsigned int i = 0; i < TWIDDLE_TABLE_SIZE; i++) {
        twiddle[i].real = cosf(PI * i / TWIDDLE_TABLE_SIZE);
        twiddle[i].imag = sinf(PI * i / TWIDDLE_TABLE_SIZE);
    }
}

void fftstockham(fcomplex *x, fcomplex *y, unsigned int N)
{
    fcomplex *y_ini = y;
    /* For q=1 to log_2(N) */
    for (unsigned int L = 2; L <= N; L <<= 1) {
        
        /* Swap pointers */
        fcomplex *tmp = x;
        x = y;
        y = tmp;

        unsigned int Ls = L >> 1;
        unsigned int r = N / L;
        unsigned int rs = N / Ls;

        for (unsigned int j = 0; j < Ls; j++) {
            /* Twiddle factor */
            fcomplex w = twiddle[j * TWIDDLE_TABLE_SIZE / Ls];
            
            for (unsigned int k = 0; k < r; k++) {
                fcomplex t = fcmul(w, y[j * rs + k + r]);
                x[(j * r) + k] = fcadd(y[(j * rs) + k], t);
                x[((j + Ls) * r) + k] = fcsub(y[(j * rs) + k], t);
            }
        }
    }
    /* If log_2(N) is odd, result values are stored on additional workspace y */
    if (y != y_ini) {
        for (unsigned int i = 0; i < N; i++) {
            y[i] = x[i];
        }
    }
}

extern void fftstockham_asm(fcomplex *x, fcomplex *y, unsigned int N);

void fft(fcomplex *x, fcomplex *y, unsigned int N)
{
    #ifdef __FFT_SSE__
        fftstockham_asm(x, y, N);
    #else
        fftstockham(x, y, N);
    #endif

}

void fct(fcomplex *x, fcomplex *y, unsigned int N)
{
    /* v(n) : reordered x(n) */
    fcomplex *v = y;
    for (unsigned int n = 0; n < N / 2; n++) {
        v[n]         = x[2 * n];     /* x_even */
        v[N - 1 - n] = x[2 * n + 1]; /* x_odd in reverse */
    }

    /* DFT of v(n), V(k) */
    fft(v, x, N);

    /* DCT = Cc(k) = C(k) - iC(N - k) = 2 * W_4N^k * V(k) */
    for (unsigned int k = 0; k < N / 2; k++) {
        fcomplex wk, Cck;
        wk = (fcomplex) {2 * cosf(k * PI / (2 * N)), 2 * sinf(k * PI / (2 * N))};  /* 2 * W_2N^k */

        /* Calculate Cc(omplex)(k) */
        Cck = fcmul(wk, v[k]);
        
        x[k].real         = Cck.real;
        x[N - 1 - k].real = Cck.imag;
    }
}