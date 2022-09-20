#include "matrix.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** -------------------------------------------------------------------------------
 *  Float Matrix
 *  -------------------------------------------------------------------------------
 */

matrixf *matrixf_new(unsigned int n_rows, unsigned int n_cols, int order)
{
    matrixf *mat = malloc(1 * sizeof(matrixf));
    if (!mat) {
        exit(-1);
    }
    mat->data = (float*)calloc(n_rows * n_cols, sizeof(float));
    if (!mat->data) {
        exit(-1);
    }
    mat->rows = n_rows;
    mat->cols = n_cols;
    mat->size = n_rows * n_cols;
    mat->order = order;

    return mat;
}

void matrixf_free(matrixf *mat)
{
    free(mat->data);
    free(mat);
}

void matrixf_set(matrixf *mat, float value)
{
    for (unsigned int i = 0; i < mat->rows; i++) {
        for (unsigned int j = 0; j < mat->cols; j++) {
            *matrixf_at(mat, i, j) = value;
        }
    }
}

float *matrixf_at(matrixf *mat, unsigned int row, unsigned int col)
{
    /* Check bounds */
    if (row >= mat->rows || col >= mat->cols) {
        exit(-1);
    }
    return mat->order == ROW_MAJOR ? mat->data + (row * mat->cols + col) : mat->data + (col * mat->rows + row); 
}

matrixf *matrixf_dot_fast(matrixf *m1, matrixf *m2, unsigned int order)
{
    /* Check dimensions and order match */
    if (m1->cols != m2->rows || m1->order == COL_MAJOR || m2->order == ROW_MAJOR) {
        exit(-1);
    }
    matrixf *res = matrixf_new(m1->rows, m2->cols, order);

    for (unsigned int i = 0; i < m1->rows; i++) {
        float *m1_row_i = matrixf_at(m1, i, 0);
        for (unsigned int j = 0; j < m2->cols; j++) {
            float *m2_col_j = matrixf_at(m2, 0, j);
            *matrixf_at(res, i, j) = vectorf_dot(m1_row_i, m2_col_j, m1->cols);
        }
    }
    return res;
}

void matrixf_mul(matrixf *m1, matrixf *m2, matrixf *result)
{
    /* Check dimensions */
    if (m1->rows != m2->rows || m1->cols != m2->cols || m1->size != result->size) {
        exit(-1);
    }

    vectorf_mul(m1->data, m2->data, result->data, result->size);
}

void matrixf_smul(matrixf *mat, float scalar, matrixf *result)
{
    /* Check result matrix is big enough */
    if (mat->size != result->size) {
        exit(-1);
    }

    vectorf_smul(mat->data, scalar, result->data, result->size);
}

/** -------------------------------------------------------------------------------
 *  Float complex matrix 
 *  -------------------------------------------------------------------------------
 */ 

matrixfc *matrixfc_new(unsigned int n_rows, unsigned int n_cols, int order)
{
    matrixfc *mat = malloc(1 * sizeof(matrixfc));
    if (!mat) {
        exit(-1);
    }

    mat->data = (fcomplex*)calloc(n_rows * n_cols, sizeof(fcomplex));
    mat->rows = n_rows;
    mat->cols = n_cols;
    mat->size = n_rows * n_cols;
    mat->order = order;

    return mat;
}

void matrixfc_free(matrixfc *mat)
{
    free(mat->data);
    free(mat);
}

fcomplex *matrixfc_at(matrixfc *mat, unsigned int row, unsigned int col)
{
    /* Check bounds */
    if (row >= mat->rows || col >= mat->cols) {
        exit(-1);
    }

    return mat->order == ROW_MAJOR ? mat->data + (row * mat->cols + col) : mat->data + (col * mat->rows + row);   
}