#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include <stdlib.h>
#include <math.h>
#include "fcomplex.h"

#define PI 3.14159265358979323846

/**
 * @brief Calculate twiddle factors to perform fft.
 * 
 * Allocates memory user should free.
 * 
 * @param fft_size 
 * @return fcomplex* 
 */
fcomplex *twiddle_factors(unsigned int fft_size);

/**
 * @brief Stockham decimation in time Fast Fourier Transform.
 * 
 * Result of the transform is stored in input signal array.
 * 
 * Reference:
 * VAN LOAN, Charles. Computational frameworks for the fast Fourier transform. Society for Industrial and Applied Mathematics, 1992. p.57
 * 
 * @param signal
 * @param workspace Additional workspace (size signal_length) required to compute the transform
 * @param twiddle Precalculated twiddle factors
 * @param signal_length
 */
void fft(fcomplex *signal, fcomplex *workspace, fcomplex *twiddle, unsigned int signal_length);



/**
 * @brief Fast cosine transform using forward fft
 * 
 * Result of the transform is stored in input signal array, signal length(N) MUST be a power of 2.
 * 
 * Reference:
 * MAKHOUL, John. A fast cosine transform in one and two dimensions. IEEE Transactions on Acoustics, Speech, and Signal Processing, 1980, vol. 28, no 1, p. 27-34.
 * https://www.nayuki.io/page/fast-discrete-cosine-transform-algorithms
 * 
 * @param signal
 * @param workspace Additional workspace (size N) required to compute the transform
 * @param signal_length
 */
void fct(float *signal, unsigned int signal_length);

#endif /* __TRANSFORMS_H__ */