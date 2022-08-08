#ifndef __FCOMPLEX_H__
#define __FCOMPLEX_H__

#include "config.h"
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

int fcequ(fcomplex x, fcomplex y, float epsilon);

#endif /* __FCOMPLEX_H__ */