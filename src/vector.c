#include "vector.h"

fcomplex fcomplex_vector_dot(fcomplex *x, fcomplex *y, unsigned int length)
{
    #ifdef __VECTOR_SSE__

    return fcomplex_vector_dot_asm(fcomplex *x, fcomplex *y, unsigned int length);

    #else

    fcomplex res = {0 , 0};
    for (unsigned int i = 0; i < length; i++) {
        res = fcadd(res, fcmul(x[i], y[i]));
    }
    return res;

    #endif
}

void fcomplex_vector_mul(fcomplex *x, fcomplex *y, unsigned int length)
{
    #ifdef __VECTOR_SSE__
    
    fcomplex_vector_mul_asm(x, y, length);

    #else

    for (unsigned int i = 0; i < length; i++) {
        x[i] = fcmul(x[i], y[i]);
    }

    #endif
}

void float_vector_mul(float *x, float *y, unsigned int length)
{
    #ifdef __VECTOR_SSE__

    float_vector_mul_asm(x, y, length);

    #else
    
    for (unsigned int i = 0; i < length; i++) {
        x[i] = x[i] * y[i];
    }

    #endif
}