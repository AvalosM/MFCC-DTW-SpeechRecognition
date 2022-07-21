#include "transforms.h"

fcomplex twiddle[TWIDDLE_TABLE_SIZE];
static int twiddle_initialized = 0;

void twiddle_init()
{
    for (unsigned int j = 0; j < TWIDDLE_TABLE_SIZE; j++) {
        twiddle[j].real = cosf(-2 * PI * j / TWIDDLE_TABLE_SIZE);
        twiddle[j].imag = sinf(-2 * PI * j / TWIDDLE_TABLE_SIZE);
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
            fcomplex w = twiddle[j * TWIDDLE_TABLE_SIZE / L];
            
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

extern void fftstockham_asm(fcomplex *x, fcomplex *workspace, unsigned int N, fcomplex *twiddle);

void fft(fcomplex *x, fcomplex *workspace, unsigned int N)
{
    if (twiddle_initialized == 0) {
        twiddle_init();
        twiddle_initialized = 1;
    }

    #ifdef __FFT_SSE__
        fftstockham_asm(x, workspace, N, twiddle);
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