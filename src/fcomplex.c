#include "fcomplex.h"

fcomplex fcmul(fcomplex x, fcomplex y)
{
    #ifdef __FCOMPLEX_ASM__

    return fcmul_pair(x, y);

    #else

    return (fcomplex){ x.real * y.real - x.imag * y.imag, x.real * y.imag + x.imag * y.real };

    #endif
}

fcomplex fcfmul(fcomplex x, float y)
{
    #ifdef __FCOMPLEX_ASM__
    
    return fcfmul_pair(x, y);

    #else

    return (fcomplex){x.real * y, x.imag * y};

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

fcomplex fcfdot(fcomplex *x, float *y, unsigned int length)
{
    #ifdef __FCOMPLEX_ASM__

    return fcdfot_asm(x, y, length);

    #else

    fcomplex res = {0, 0};
    for (unsigned int i = 0; i < length; i++) {
        res = fcsum(res, fcfmul(*x, *y));
    }
    return res;

    #endif
}