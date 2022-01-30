#include "test.h"
#include "../transforms.h"
#include "../io.h"

// extern void fft(fcomplex *x, fcomplex *y, unsigned int N);


void fft_allzero()
{
    twiddle_init();
    testinit("fft_allzero");

    unsigned int len = 16;
    fcomplex *signal    = (fcomplex*)calloc(len, sizeof(fcomplex));
    fcomplex *workspace = (fcomplex*)calloc(len, sizeof(fcomplex));

    fft(signal, workspace, len);

    for (unsigned int i = 0; i < len; i++) {
        ASSERT_EQ(signal[i].real, 0);
        ASSERT_EQ(signal[i].imag, 0);
    }
    free(signal);
    free(workspace);

    testpass();
}

void fft_linearity()
{
    testinit("fft_linearity");

    /**
     * The DFT is a linear operator
     * FFT(a1 x1[n] + a2 x2[n]) = a1 FFT(x1[n]) + a2 FFT(x2[n]) must hold
     */
    unsigned int len = 16;
    fcomplex *x1 = (fcomplex*)calloc(len, sizeof(fcomplex));
    fcomplex *x2 = (fcomplex*)calloc(len, sizeof(fcomplex));
    fcomplex a1 = { 3, 0 };
    fcomplex a2 = { 7, 0 };

    /* Generate two different signals */
    for (unsigned int i = 0; i < len; i++) {
        x1[i].real = sinf((2 * PI * i) / 16);
        x2[i].real = sinf(((2 * PI * i) / 8) + 2);
    }

    fcomplex *y = (fcomplex*)calloc(len, sizeof(fcomplex));
    for (unsigned int i = 0; i < len; i++) {
        y[i] = fcsum(fcmul(a1, x1[i]), fcmul(a2, x2[i]));
    }

    fcomplex *workspace = (fcomplex*)calloc(len, sizeof(fcomplex));

    fft(y, workspace, len); /* FFT(a1 x1[n] + a2 x2[n]) */
    fft(x1, workspace, len);
    fft(x2, workspace, len);
    for (unsigned int i = 0; i < len; i++) {
        x1[i] = fcsum(fcmul(a1, x1[i]), fcmul(a2, x2[i])); /* a1 FFT(x1[n]) + a2 FFT(x2[n]) */
    }

    for (unsigned int i = 0; i < len; i++) {
        fcomplex actual = fcsub(x1[i], y[i]);
        ASSERT(actual.real < (float)0.00001);
        ASSERT(actual.imag < (float)0.00001);
    }

    free(x1);
    free(x2);
    free(y);
    free(workspace);

    testpass();
}

void fft_sinewave()
{
    twiddle_init();
    testinit("fft_sinewave");

    unsigned int len = 16;
    unsigned int samplerate = len;
    fcomplex *sinewave  = malloc(len * sizeof(fcomplex));
    fcomplex *workspace = malloc(len * sizeof(fcomplex));

    /* 4Hz sine wave, 2s signal */
    for (unsigned int i = 0; i < len; i++) {
        sinewave[i].real = sinf((2 * PI * i) / (samplerate / 4));
    }
    savesignalc(TEST_RES_DIR("fft_sinewave.csv"), sinewave, len);

    fft(sinewave, workspace, len);
    savesignalc(TEST_RES_DIR("fft_fftsinewave.csv"), sinewave, len);

    /* Bin corresponding with periodogram peak should be found at 4hz(4th bin) */
    unsigned int max_bin = 0;
    for (unsigned int i = 1; i < len / 2 + 1; i++) {
        max_bin = powf(fcabs(sinewave[i]), 2) > powf(fcabs(sinewave[max_bin]), 2) ? i : max_bin;
    }
    unsigned int expected = 4;
    ASSERT_EQ(max_bin, expected);

    free(sinewave);
    free(workspace);

    testpass();
}

void fft_wavesum()
{
    testinit("fft_wavesum");

    unsigned int len = 64;
    unsigned int samplerate = len;
    fcomplex *signal = (fcomplex*)calloc(len, sizeof(fcomplex));
    fcomplex *workspace = malloc(len * sizeof(fcomplex));

    /* 4hz + 8hz sine waves 2s signal */
    for (unsigned int i = 0; i < len; i++) {
        signal[i].real = sinf((2 * PI * i) / (samplerate / 4)) + sinf((2 * PI * i) / (samplerate / 8));
    }
    savesignalc(TEST_RES_DIR("fft_wavesum.csv"), signal, len);

    fft(signal, workspace, len);
    savesignalc(TEST_RES_DIR("fft_fftwavesum.csv"), signal, len);

    /* Bin corresponding with periodogram peaks should be found at 4hz(4th bin) and 8hz(8th bin)*/
    unsigned int max_bin = 0;
    unsigned int second_max_bin = 0;
    for (unsigned int i = 1; i < len / 2 + 1; i++) {
        if (powf(fcabs(signal[i]), 2) > powf(fcabs(signal[max_bin]), 2)){
            max_bin = i;
        } else if (powf(fcabs(signal[i]), 2) > powf(fcabs(signal[second_max_bin]), 2)){
            second_max_bin = i;
        }
    }

    unsigned int expected_max = 4;
    unsigned int expected_second_max = 8;

    ASSERT_EQ(max_bin, expected_max);
    ASSERT_EQ(second_max_bin, expected_second_max);

    free(signal);
    free(workspace);

    testpass();
}


void fft_testsuite()
{
    suiteinit("FFT");
    fft_allzero();
    fft_linearity();
    fft_sinewave();
    fft_wavesum();
}