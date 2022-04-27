#ifndef __FCOMPLEX_H__
#define __FCOMPLEX_H__

#include <math.h>

typedef struct fcomplex 
{
    float real;
    float imag;
} fcomplex;

fcomplex fcmul(fcomplex x, fcomplex y);

fcomplex fcadd(fcomplex x, fcomplex y);

fcomplex fcsub(fcomplex x, fcomplex y);

float fcabs(fcomplex x);

/**
 * Multiply two pairs of complex numbers (ASM/SSE implementation)
 * Reference: Intel® 64 and IA-32 Architectures Optimization Reference Manual
 */
extern fcomplex fcmul_pair_asm(fcomplex x, fcomplex y);

#endif /* __FCOMPLEX_H__ */