#include "fcomplex.h"

fcomplex fcmul(fcomplex x, fcomplex y)
{
    return (fcomplex){ x.real * y.real - x.imag * y.imag, x.real * y.imag + x.imag * y.real };
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
    return (float)sqrtf(powf(x.real, 2) + powf(x.imag, 2));
}

int fcequ(fcomplex x, fcomplex y, float epsilon)
{
    fcomplex sub = fcsub(x, y);
    return sub.real < epsilon && sub.imag < epsilon;
}