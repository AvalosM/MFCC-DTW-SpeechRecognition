#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "fcomplex.h"

fcomplex fcomplex_vector_dot(fcomplex *x, fcomplex *y, unsigned int length);

void fcomplex_vector_mul(fcomplex *x, fcomplex *y, unsigned int length);

float float_vector_dot(float *x, float *y, unsigned int length);

void float_vector_mul(float *x, float *y, unsigned int length);

/* 
 * ASM/SSE implementations
 */
extern fcomplex fcomplex_vector_dot_asm(fcomplex *x, fcomplex *y, unsigned int length);
extern void fcomplex_vector_mul_asm(fcomplex *x, fcomplex *y, unsigned int length);
extern void float_vector_mul_asm(float *x, float *y, unsigned int length);

#endif /* __VECTOR_H__ */