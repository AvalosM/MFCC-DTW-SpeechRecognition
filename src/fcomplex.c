#include "fcomplex.h"

fcomplex fcmul(fcomplex x, fcomplex y)
{
    #ifdef __FCOMPLEX_ASM__

    return cmul_pair(x, y);

    #else

    return (fcomplex){ x.real * y.real - x.imag * y.imag, x.real * y.imag + x.imag * y.real };

    #endif
}

fcomplex fcsum(fcomplex x, fcomplex y)
{
    return (fcomplex){ x.real + y.real, x.imag + y.imag };
}

fcomplex fcsub(fcomplex x, fcomplex y)
{
    return (fcomplex){ x.real - y.real, x.imag - y.imag };
}

float fcabs(fcomplex x)
{
    return sqrtf(powf(x.real, 2) + powf(x.imag, 2));
}

fcomplex fcdot(fcomplex *x, fcomplex *y, unsigned int length)
{
    #ifdef __FCOMPLEX_ASM__

    return fcdot_asm(x, y, length);

    #else

    fcomplex res = {0, 0};
    for (unsigned int i = 0; i < length; i++) {
        res = fcsum(res, fcmul(*x,*y));
    }
    return res;

    #endif
}