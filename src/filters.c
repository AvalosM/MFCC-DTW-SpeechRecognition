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
float *hamming_window(unsigned int length)
{
    float *window = malloc(length * sizeof(float));
    for (unsigned int i = 0; i < length; i++) {
        window[i] = 0.54 - 0.46 * cos((2 * PI * i)/length);
    }
    
    return window;
}

/* http://practicalcryptography.com/miscellaneous/machine-learning/guide-mel-frequency-cepstral-coefficients-mfccs/#eqn1 */
/* M(f) = 1125 ln(1 + f/700) freq -> mel */
float hz2mel(float freq)
{
    return 1125 * logf(1 + (freq / 700));
}

/* M^-1(m) = 700(exp(m/1125) - 1) mel -> freq */
float mel2hz(float mel)
{
    return 700 * (expf(mel / 1125) - 1);
}

matrixf *mel_filterbank(float lower_freq, float upper_freq, unsigned int samplerate, unsigned int fft_size)
{
    /* Convert lower and upper bounds to Mel scale */
    float lower_freq_mel = hz2mel(lower_freq);
    float upper_freq_mel = hz2mel(upper_freq);
    float step_freq_mel = (upper_freq_mel - lower_freq_mel) / (MEL_FILTER_NUM + 1);

    /* Linearly spaced (in Mel scale) filter centre-frequencies */
    float *fft_bins = malloc((MEL_FILTER_NUM + 2)* sizeof(float));
    for (unsigned int i = 0; i < MEL_FILTER_NUM + 2; i++) {
        fft_bins[i] = lower_freq_mel + (step_freq_mel * i);
    }

    /* Round to nearest fft frequency bin */
    for (unsigned int i = 0; i < MEL_FILTER_NUM + 2; i++) {
        fft_bins[i] = floorf((fft_size + 1) * mel2hz(fft_bins[i]) / samplerate);
    }

    /* Create filterbank */
    unsigned int filter_length = fft_size / 2 + 1;
    matrixf *filterbank = matrixf_new(filter_length, MEL_FILTER_NUM, COL_MAJOR);
    for (unsigned int m = 1; m < MEL_FILTER_NUM + 1; m++) {
        float *filter_m = matrixf_at(filterbank, 0, m - 1);
        for (unsigned int k = 0; k < filter_length; k++) {
            if (k < fft_bins[m - 1]) {
                filter_m[k] = 0;
            } else if (k <= fft_bins[m]) {
                filter_m[k] = (k - fft_bins[m - 1]) / (fft_bins[m] - fft_bins[m - 1]);
            } else if (k <= fft_bins[m + 1]) {
                filter_m[k] = (fft_bins[m + 1] - k) / (fft_bins[m + 1] - fft_bins[m]);
            } else {
                filter_m[k] = 0;
            }
        }
    }

    free(fft_bins);
    return filterbank;
}

float *cepstral_lifter(unsigned int liftering_coefficient, unsigned int mel_filter_count)
{
    float *lifter = malloc(mel_filter_count * sizeof(float));
    for (unsigned int i = 0; i < mel_filter_count; i++) {
        lifter[i] = 1.0 + (liftering_coefficient / 2.0) * sin(PI * i / liftering_coefficient);
    }

    return lifter;
}