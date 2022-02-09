#ifndef __FCOMPLEX_H__
#define __FCOMPLEX_H__

#include <math.h>

typedef struct __attribute__((packed)) fcomplex 
{
    float real;
    float imag;
} fcomplex;

extern fcomplex cmul_pair(fcomplex x, fcomplex y);

fcomplex fcmul(fcomplex x, fcomplex y);

fcomplex fcsum(fcomplex x, fcomplex y);

fcomplex fcsub(fcomplex x, fcomplex y);

float fcabs(fcomplex x);

extern fcomplex fcdot_asm(fcomplex *x, fcomplex *y, unsigned int length);

fcomplex fcdot(fcomplex *x, fcomplex *y, unsigned int length);

#endif /* __FCOMPLEX_H__ */