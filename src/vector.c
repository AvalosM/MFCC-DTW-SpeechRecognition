#include "vector.h"

float vectorf_dot(float *v1, float *v2, unsigned int length)
{
    #ifdef __VECTOR_AVX__

    return vectorf_dot_asm(v1, v2, length);

    #else 

    float result = 0;
    for (unsigned int i = 0; i < length; i++) {
        result += v1[i] * v2[i];
    }

    return result;
    
    #endif
}

void vectorf_mul(float *v1, float *v2, float *result, unsigned int length)
{
    #ifdef __VECTOR_AVX__

    vectorf_mul_asm(v1, v2, result, length);

    #else

    for (unsigned int i = 0; i < length; i++) {
        result[i] = v1[i] * v2[i];
    }

    #endif
}

void vectorf_smul(float *vec, float scalar, float *result, unsigned int length)
{
    #ifdef __VECTOR_AVX__

    vectorf_smul_asm(vec, scalar, result, length);

    #else

    for (unsigned int i = 0; i < length; i++) {
        result[i] = vec[i] * scalar;
    }

    #endif
}

void vectorf_sub(float *v1, float *v2, float *result, unsigned int length)
{
    #ifdef __VECTOR_AVX__

    vectorf_sub_asm(v1, v2, result, length);

    #else

    for (unsigned int i = 0; i < length; i++) {
        result[i] = v1[i] - v2[i];
    }

    #endif
}

void vectorf_to_fc(float *vec, fcomplex *result, unsigned int length)
{
    #ifdef __VECTOR_AVX__

    vectorf_to_fc_asm(vec, result, length);

    #else

    for (unsigned int i = 0; i < length; i++) {
        result[i].real = vec[i];
        result[i].imag = 0;
    }

    #endif
}

void vectorfc_mul(fcomplex *v1, fcomplex *v2, fcomplex *result, unsigned int length)
{
    #ifdef __VECTOR_AVX__

    vectorfc_mul_asm(v1, v2, result, length);

    #else

    for (unsigned int i = 0; i < length; i++) {
        result[i] = fcmul(v1[i], v2[i]);
    }

    #endif
}

void vectorfc_add(fcomplex *v1, fcomplex *v2, fcomplex *result, unsigned int length)
{
    #ifdef __VECTOR_AVX__

    vectorfc_add_asm(v1, v2, result, length);

    #else

    for (unsigned int i = 0; i < length; i++) {
        result[i] = fcadd(v1[i], v2[i]);
    }

    #endif
}

void vectorfc_sub(fcomplex *v1, fcomplex *v2, fcomplex *result, unsigned int length)
{
    #ifdef __VECTOR_AVX__

    vectorfc_sub_asm(v1, v2, result, length);

    #else

    for (unsigned int i = 0; i < length; i++) {
        result[i] = fcsub(v1[i], v2[i]);
    } 

    #endif
}

void vectorfc_abs(fcomplex *vec, float *result, unsigned int length)
{
    #ifdef __VECTOR_AVX__

    vectorfc_abs_asm(vec, result, length);

    #else

    for (unsigned int i = 0; i < length; i++) {
        result[i] = fcabs(vec[i]);
    }

    #endif
}