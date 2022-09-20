#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <math.h>
#include "matrix.h"
#define PI 3.14159265358979323846
#define MEL_FILTER_NUM 40
/**
 * @brief Pre-emphasis filter: Y[n] = X[n] - 0.95 * X[n - 1] where X is the input signal and Y the output
 * 
 * @param signal signal to filter
 * @param signal_length
 */
void preemphasis(float *signal, unsigned int signal_length);

/**
 * @brief Initialize hamming window
 * 
 * This function allocates memory that user should free
 * 
 * @param length
 * @return float* 
 */
float *hamming_window(unsigned int length);

/**
 * @brief Initialize mel-filterbank
 * 
 * This function allocates memory that user should free
 * 
 * @param lower_freq 
 * @param upper_freq 
 * @param filter_num 
 * @param samplerate 
 * @param fft_size 
 * @return float* 
 */
matrixf *mel_filterbank(float lower_freq, float upper_freq, unsigned int samplerate, unsigned int fft_size);

float *cepstral_lifter(unsigned int liftering_coefficient, unsigned int mel_filter_count);

#endif /* __FILTERS_H__ */