#include "transforms.h"

fcomplex twiddle[TWIDDLE_TABLE_SIZE];

void twiddle_init()
{
    for (unsigned int i = 0; i < TWIDDLE_TABLE_SIZE; i++) {
        twiddle[i].real = cosf(PI * i / TWIDDLE_TABLE_SIZE);
        twiddle[i].imag = sinf(PI * i / TWIDDLE_TABLE_SIZE);
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
            fcomplex w = twiddle[j * TWIDDLE_TABLE_SIZE / Ls];
            
            for (unsigned int k = 0; k < r; k++) {
                fcomplex t = fcmul(w, workspace[j * rs + k + r]);
                x[(j * r) + k] = fcadd(workspace[(j * rs) + k], t);
                x[((j + Ls) * r) + k] = fcsub(workspace[(j * rs) + k], t);
            }
        }
    }
    /* If log_2(N) is odd, result values are stored on additional workspace workspace */
    if (workspace != y_ini) {
        for (unsigned int i = 0; i < N; i++) {
            workspace[i] = x[i];
        }
    }
}

extern void fftstockham_asm(fcomplex *x, fcomplex *workspace, unsigned int N);

void fft(fcomplex *x, fcomplex *workspace, unsigned int N)
{
    #ifdef __FFT_SSE__
        fftstockham_asm(x, workspace, N);
    #else
        fftstockham(x, workspace, N);
    #endif

}

void fct(float *signal, unsigned int signal_length)
{
    fcomplex *v = calloc(signal_length, sizeof(fcomplex));
    fcomplex *workspace = malloc(signal_length * sizeof(fcomplex));

    /* v(n) : reordered signal(n) */
    for (unsigned int n = 0; n < signal_length / 2; n++) {
        v[n].real                     = signal[2 * n];     /* x_even */
        v[signal_length - 1 - n].real = signal[2 * n + 1]; /* x_odd in reverse */
    }

    /* DFT of v(n), V(k) */
    fft(v, workspace, signal_length);

    /* DCT = Cc(k) = C(k) - iC(N - k) = 2 * W_4N^k * V(k) */
    for (unsigned int k = 0; k < signal_length / 2; k++) {
        fcomplex wk, Cck;
        /* TODO twiddle table*/
        wk = (fcomplex) {2 * cosf(k * PI / (2 * signal_length)), 2 * sinf(k * PI / (2 * signal_length))};  /* 2 * W_2N^k */

        /* Calculate Cc(omplex)(k) */
        Cck = fcmul(wk, v[k]);
        
        signal[k]                     = Cck.real;
        signal[signal_length - 1 - k] = Cck.imag;
    }
    free(v);
    free(workspace);
}