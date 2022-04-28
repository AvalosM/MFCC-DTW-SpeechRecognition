#include <stdlib.h>
#include "filters.h"

void preemphasis(float *signal, unsigned int signal_length)
{
    float prev = signal[0];
    for (unsigned int i = 1; i < signal_length; i++) {
        float curr = signal[i];
        signal[i] = curr - 0.95f * prev;
        prev = curr;
    }
}

/* https://arxiv.org/abs/1003.4083 */
/* Hann window with a_0 ~= 0.54 */
float *hammingwindow(unsigned int length)
{
    float *window = malloc(length * sizeof(float));
    for (unsigned int i = 0; i < length; i++) {
        window[i] = 0.54 - 0.46 * cos((2 * PI * i)/length);
    }
    return window;
}

/* http://practicalcryptography.com/miscellaneous/machine-learning/guide-mel-frequency-cepstral-coefficients-mfccs/#eqn1 */
/* M(f) = 1125 ln(1 + f/700) freq -> mel */
float mel(float freq)
{
    return 1125 * logf(1 + (freq / 700));
}

/* M^-1(m) = 700(exp(m/1125) - 1) mel -> freq */
float imel(float mel)
{
    return 700 * (expf(mel / 1125) - 1);
}

matrixfc *melfilterbank(float lower_freq, float upper_freq, unsigned int samplerate, unsigned int fft_size)
{
    /* Convert lower and upper bounds to Mel scale */
    float lower_freq_mel = mel(lower_freq);
    float upper_freq_mel = mel(upper_freq);
    float step_freq_mel = (upper_freq_mel - lower_freq_mel) / (MEL_FILTER_NUM + 1);

    /* Linearly spaced (in Mel scale) filter centre-frequencies */
    float *fft_bins = malloc((MEL_FILTER_NUM + 2)* sizeof(float));
    for (unsigned int i = 0; i < MEL_FILTER_NUM + 2; i++) {
        fft_bins[i] = imel(lower_freq_mel + (step_freq_mel * i));
    }

    /* Round to nearest fft frequency bin */
    for (unsigned int i = 0; i < MEL_FILTER_NUM + 2; i++) {
        fft_bins[i] = floorf((fft_size + 1) * fft_bins[i] / samplerate);
    }

    /* Create filterbank */
    unsigned int bin_count = (fft_size / 2) + 1; /* Bins corresponding with the positive part of the spectrum (first half + 1) */
    matrixfc *filterbank = matrixfc_new(MEL_FILTER_NUM, bin_count, COL_MAJOR);
    for (unsigned int m = 1; m < MEL_FILTER_NUM + 1; m++) {
        for (unsigned int k = 0; k < bin_count; k++){
            fcomplex *curr_position = matrixf_at(filterbank, (m-1), k);
            (*curr_position).imag = 0;
            if (k < fft_bins[m - 1]) {
                (*curr_position).real = 0;
            } else if (k <= fft_bins[m]) {
                (*curr_position).real = (k - fft_bins[m - 1]) / (fft_bins[m] - fft_bins[m - 1]);
            } else if (k <= fft_bins[m + 1]) {
                (*curr_position).real = (fft_bins[m + 1] - k) / (fft_bins[m + 1] - fft_bins[m]);
            } else {
                (*curr_position).real = 0;
            }
        }
    }

    free(fft_bins);
    return filterbank;
}