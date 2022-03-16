#ifndef __FCOMPLEX_H__
#define __FCOMPLEX_H__

#include <math.h>

typedef struct __attribute__((packed)) fcomplex 
{
    float real;
    float imag;
} fcomplex;

fcomplex fcmul(fcomplex x, fcomplex y);

fcomplex fcfmul(fcomplex x, float y);

fcomplex fcsum(fcomplex x, fcomplex y);

fcomplex fcsub(fcomplex x, fcomplex y);

float fcabs(fcomplex x);

/**
 * @brief Dot product between fcomplex and float vector
 * 
 * @param *x pointer to fcomplex array
 * @param *y pointer to float array
 * @param length Array length
 */
fcomplex fcfdot(fcomplex *x, float *y, unsigned int length);



extern fcomplex fcmul_pair(fcomplex x, fcomplex y);
extern fcomplex fcfdot_asm(fcomplex *x, float *y, unsigned int length);

#endif /* __FCOMPLEX_H__ */