#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "fcomplex.h"
#define __MATRIX_SSE__

#define ROW_MAJOR 1
#define COL_MAJOR 0

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

matrixfc *matrixfc_dot_fast(matrixfc *m1, matrixfc *m2);

void matrixfc_abs_r(matrixfc *mat);

void matrixfc_smult_r(matrixfc *mat, float scalar);

void matrixfc_mul_r(matrixfc *m1, matrixfc *m2);

/* 
 * ASM/SSE implementations
 */
extern fcomplex vectorfc_dot_asm(fcomplex *v1, fcomplex *v2, unsigned int length);

/**
 * float matrix
 */



#endif /* __MATRIX_H__ */