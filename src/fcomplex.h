#ifndef __FCOMPLEX_H__
#define __FCOMPLEX_H__

#include <math.h>

typedef struct __attribute__((packed)) fcomplex 
{
    float real;
    float imag;
} fcomplex;

fcomplex fcmul(fcomplex x, fcomplex y);

fcomplex fcsum(fcomplex x, fcomplex y);

fcomplex fcsub(fcomplex x, fcomplex y);

float fcabs(fcomplex x);

#endif /* __FCOMPLEX_H__ */