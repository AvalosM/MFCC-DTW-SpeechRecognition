#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "fcomplex.h"
/* 
 * ASM/AVX implementations
 */
extern float vectorf_dot_asm(float *v1, float *v2, unsigned int length);
extern void vectorf_mul_asm(float *v1, float *v2, float *result, unsigned int length);
extern void vectorf_add_asm(float *v1, float *v2, float *result, unsigned int length);
extern void vectorf_sub_asm(float *v1, float *v2, float *result, unsigned int length);
extern void vectorf_smul_asm(float *vec, float scalar, float *result, unsigned int length);

extern void vectorfc_mul_asm(fcomplex *v1, fcomplex *v2, fcomplex *result, unsigned int length);
extern void vectorfc_add_asm(fcomplex *v1, fcomplex *v2, fcomplex *result, unsigned int length);
extern void vectorfc_sub_asm(fcomplex *v1, fcomplex *v2, fcomplex *result, unsigned int length);

#endif /* __VECTOR_H__ */