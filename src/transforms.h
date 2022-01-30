#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include <stdlib.h>
#include <math.h>
#include "fcomplex.h"


#define PI 3.14159265358979323846
#define TWIDDLE_TABLE_SIZE 1024
#define __SSE_ASM__

/**
 * @brief Init twiddle factor table
 * 
 * Precalculate twiddle factors from e^(-2PIi * 0 / FFT_POINT_MAX) to e^(-2PIi * TWIDDLE_TABLE_SIZE / FFT_POINT_MAX)
 */
void twiddle_init();


/**
 * @brief Stockham decimation in time Fast Fourier Transform 
 *
 * Result of the transform is stored in input signal array, signal length(N) MUST be a power of 2 and <= 2048
 * 
 * Reference:
 * VAN LOAN, Charles. Computational frameworks for the fast Fourier transform. Society for Industrial and Applied Mathematics, 1992. p.57
 * 
 * @param x Input signal
 * @param y Additional workspace (size N) required to compute the transform
 * @param N Signal length 
 */
void fftstockham(fcomplex *x, fcomplex *y, unsigned int N);

extern void fftstockham_asm(fcomplex *x, fcomplex *y, unsigned int N);

void fft(fcomplex *x, fcomplex *y, unsigned int N);

#endif /* __TRANSFORMS_H__ */