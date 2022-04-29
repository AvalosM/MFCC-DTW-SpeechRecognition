#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include <stdlib.h>
#include <math.h>
#include "fcomplex.h"

#define PI 3.14159265358979323846
#define TWIDDLE_TABLE_SIZE 1024

/**
 * @brief Init twiddle factor table
 * 
 * Precalculate twiddle factors from e^(-2PIi * 0 / FFT_POINT_MAX) to e^(-2PIi * TWIDDLE_TABLE_SIZE / FFT_POINT_MAX)
 */
void twiddle_init();

/**
 * @brief Stockham decimation in time Fast Fourier Transform.
 * 
 * Implementation depends on definition of __FFT_SSE__.
 *
 * Result of the transform is stored in input signal array, signal length(N) MUST be a power of 2 and <= 2 * TWIDDLE_TABLE_SIZE.
 * 
 * Reference:
 * VAN LOAN, Charles. Computational frameworks for the fast Fourier transform. Society for Industrial and Applied Mathematics, 1992. p.57
 * 
 * @param x Input signal
 * @param workspace Additional workspace (size N) required to compute the transform
 * @param N Signal length 
 */
void fft(fcomplex *signal, fcomplex *workspace, unsigned int signal_length);

/**
 * @brief Fast cosine transform using forward fft
 * 
 * Result of the transform is stored in input signal array, signal length(N) MUST be a power of 2.
 * 
 * Reference:
 * MAKHOUL, John. A fast cosine transform in one and two dimensions. IEEE Transactions on Acoustics, Speech, and Signal Processing, 1980, vol. 28, no 1, p. 27-34.
 * https://www.nayuki.io/page/fast-discrete-cosine-transform-algorithms
 * 
 * @param x Input signal
 * @param workspace Additional workspace (size N) required to compute the transform
 * @param N Signal length
 */
void fct(float *signal, unsigned int signal_length);

#endif /* __TRANSFORMS_H__ */