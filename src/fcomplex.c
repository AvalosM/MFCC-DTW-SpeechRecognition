#include "fcomplex.h"

fcomplex fcmul(fcomplex x, fcomplex y)
{
    #ifdef __FCOMPLEX_SSE__

    return fcmul_pair_asm(x, y);

    #else

    return (fcomplex){ x.real * y.real - x.imag * y.imag, x.real * y.imag + x.imag * y.real };

    #endif
}

fcomplex fcadd(fcomplex x, fcomplex y)
{
    return (fcomplex){ x.real + y.real, x.imag + y.imag };
}

fcomplex fcsub(fcomplex x, fcomplex y)
{
    return (fcomplex){ x.real - y.real, x.imag - y.imag };
}

float fcabs(fcomplex x)
{
    #ifdef __FCOMPLEX_SSE__

    return fcabs_pair_asm(x);

    #else

    return (float)sqrtf(powf(x.real, 2) + powf(x.imag, 2));

    #endif
}