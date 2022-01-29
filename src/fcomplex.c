#include "fcomplex.h"

fcomplex fcmul(fcomplex x, fcomplex y)
{
    return (fcomplex){ x.real * y.real - x.imag * y.imag, x.real * y.imag + x.imag * y.real };
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