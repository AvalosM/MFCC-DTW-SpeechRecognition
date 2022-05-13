#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "fcomplex.h"
// #define __MATRIX_SSE__

#define ROW_MAJOR 1
#define COL_MAJOR 0

/**
 * float matrix
 */
typedef struct matrixf {
    float *data;
    unsigned int rows;
    unsigned int cols;
    int order;
} matrixf;

matrixf *matrixf_new(unsigned int n_rows, unsigned int n_cols, int order);

void matrixf_free(matrixf *mat);

void matrixf_set(matrixf *mat, float value);

float *matrixf_at(matrixf *mat, unsigned int row, unsigned int col);

/**
 * @brief Float matrix fast dot product
 * 
 * Requires m1 to be row-major and m2 column-major
 * Rows in m1 and columns in m2 being contiguous in memory allows for faster computation
 * Allocates matrix user should free
 * 
 * @param m1 Row-major float matrix
 * @param m2 Column-major float matrix
 * @return matrixf* 
 */
matrixf *matrixf_dot_fast(matrixf *m1, matrixf *m2);

/**
 * @brief Entrywise matrix multiplication
 * 
 * Matrices must have the same dimension
 * 
 * @param m1
 * @param m2
 */
void matrixf_mul_r(matrixf *m1, matrixf *m2);

/**
 * @brief Matrix multiplication by scalar
 * 
 * @param mat
 * @param scalar 
 */
void matrixf_smul_r(matrixf *mat, float scalar);

float vectorf_dist(float *v1, float *v2, unsigned int length);

/**
 * fcomplex matrix
 */
typedef struct matrixfc {
    fcomplex *data;
    unsigned int rows;
    unsigned int cols;
    int order;
} matrixfc;

matrixfc *matrixfc_new(unsigned int n_rows, unsigned int n_cols, int order);

void matrixfc_free(matrixfc *mat);

fcomplex *matrixfc_at(matrixfc *mat, unsigned int row, unsigned int col);

/**
 * @brief Entrywise absolute value of an fcomplex matrix
 * 
 * Allocates matrix user shoud free
 * 
 * @param mat 
 * @return matrixf* Pointer to float matrix containing absolute values 
 */
matrixf *matrixfc_abs(matrixfc *mat);


/* 
 * ASM/SSE implementations
 */
extern float vectorf_dot_asm(float *v1, float *v2, unsigned int length);
extern void vectorf_mul_r_asm(float *v1, float *v2, unsigned int length);
extern void vectorf_smul_r_asm(float *vec, float scalar, unsigned int length);
extern float vectorf_norm_asm(float *vec, unsigned int length);

#endif /* __MATRIX_H__ */