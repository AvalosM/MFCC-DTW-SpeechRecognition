#ifndef __VECTOR_H__
#define __VECTOR_H__

/* 
 * ASM/SSE implementations
 */
extern float vectorf_dot_asm(float *v1, float *v2, unsigned int length);
extern void vectorf_mul_r_asm(float *v1, float *v2, unsigned int length);
extern void vectorf_smul_r_asm(float *vec, float scalar, unsigned int length);
extern float vectorf_norm_asm(float *vec, unsigned int length);

#endif /* __VECTOR_H__ */