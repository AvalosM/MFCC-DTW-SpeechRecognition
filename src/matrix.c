#include "matrix.h"
#include <stdlib.h>

/** 
 * Float Matrix
 */

matrixf *matrixf_new(unsigned int n_rows, unsigned int n_cols, int order)
{
    matrixf *mat = calloc(1, sizeof(float));
    if (!mat) {
        exit(-1);
    }
    mat->data = calloc(n_rows * n_cols, sizeof(float));
    if (!mat->data) {
        exit(-1);
    }
    mat->rows = n_rows;
    mat->cols = n_cols;
    mat->order = order;

    return mat;
}

void matrixf_free(matrixf *mat)
{
    free(mat->data);
    free(mat);
}

float *matrixf_at(matrixf *mat, unsigned int row, unsigned int col)
{
    /* Check bounds */
    if (row >= mat->rows || col >= mat->cols) {
        exit(-1);
    }
    return mat->order == ROW_MAJOR ? mat->data + (row * mat->cols + col) : mat->data + (col * mat->rows + row); 
}

matrixf *matrixf_dot_fast(matrixf *m1, matrixf *m2)
{
    /* Check dimensions and order match */
    if (m1->cols != m2->rows || m1->order == COL_MAJOR || m2->order == ROW_MAJOR) {
        exit(-1);
    }
    matrixf *res = malloc(m1->rows * m2->cols * sizeof(float));
    if (!res) {
        exit(-1);
    }
    for (unsigned int i = 0; i < m1->rows; i++) {
        float *m1_row_i = matrixf_at(m1, i, 0);
        for (unsigned int j = 0; j < m2->cols; j++) {
            float *m2_col_j = matrixf_at(m2, 0, j);
            float scalar;
            #ifdef __MATRIX_SSE__

            scalar = vectorf_dot_asm(m1_row_i, m2_col_j, m1->cols);

            #else

            for (unsigned int k = 0; k < m1->cols; k++) {
                scalar += m1_row_i[k] * m2_col_j[k];
            }

            #endif
        }
    }
}

void matrixf_mul_r(matrixf *m1, matrixf *m2)
{
    /* Check dimensions */
    if (m1->rows != m2->rows || m1->cols != m2->cols) {
        exit(-1);
    }
    for (unsigned int i = 0; i < m1->rows; i++) {
        float *m1_row_i = matrixf_at(m1, i, 0);
        float *m2_row_i = matrixf_at(m2, i, 0);

        #ifdef __MATRIX_SSE__

        vectorf_mul_r_asm(m1_row_i, m2_row_i, m1->cols);

        #else

        for (unsigned int j = 0; j < m1->cols; j++) {
            m1_row_i[j] = m1_row_i[j] * m2_row_i[j];
        }

        #endif
    }
}

void matrixf_smul_r(matrixf *mat, float scalar)
{
    for (unsigned int i = 0; i < mat->rows; i++) {
        float *mat_row_i = matrixf_at(mat, i, 0);

        #ifdef __MATRIX_SSE__

        vectorf_smul_r_asm(mat_row_i, scalar, mat->cols);

        #else

        for (unsigned int j = 0; j < mat->cols; j++) {
            mat_row_i[j] = mat_row_i[j] * scalar;
        }

        #endif
    }
}

/**
 * Float complex matrix 
 */

matrixfc *matrixfc_new(unsigned int n_rows, unsigned int n_cols, int order)
{
    matrixfc *mat = calloc(1, sizeof(matrixfc));
    if (!mat) {
        exit(-1);
    }

    mat->data = calloc(n_rows * n_cols, sizeof(fcomplex));
    mat->rows = n_rows;
    mat->cols = n_cols;
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

matrixfc *matrixfc_dot_fast(matrixfc *m1, matrixfc *m2)
{
    /* Check dimensions and order match */
    if (m1->cols != m2->rows || m1->order == COL_MAJOR || m2->order == ROW_MAJOR) {
        exit(-1);
    }

    matrixfc *res = matrixfc_new(m1->rows, m2->cols, ROW_MAJOR);
    if (!res) {
        exit(-1);
    }

    for (unsigned int i = 0; i < m1->rows; i++) {
        fcomplex *curr_m1_row = m1->data + i * m1->cols;
        for (unsigned int j = 0; j < m2->cols; j++) {
            fcomplex *curr_m2_col = m2->data + j * m2->rows;
            fcomplex sum = {0, 0};

            #ifdef __MATRIX_SSE__
            
            sum = vectorfc_dot_asm(curr_m1_row, curr_m2_col, m1->cols);
            
            #else
            
            for (unsigned int k = 0; k < m1->cols; k++) {
                sum += fcmul(curr_m1_row[k], curr_m2_col[k]);
            }
            
            #endif /* __MATRIX_SSE__ */

            res->data[j + i * res->cols] = sum;

        }
    }
    return res;
}

matrixf *matrixfc_abs(matrixfc *mat)
{
    matrixf *res = matrixf_new(mat->rows, mat->cols, ROW_MAJOR);

    for (unsigned int i = 0; i < mat->rows; i++) {
        fcomplex *mat_row_i = matrixfc_at(mat, i, 0);
        float *res_row_i = matrixf_at(res, i, 0);

        #ifdef __MATRIX_SSE__

        *res_row_i = vectorfc_abs_asm(mat_row_i, mat->cols);
        
        #else

        for (unsigned int j = 0; j < mat->cols; j++) {
            res_row_i[j] = fcabs(mat_row_i[j])
        }

        #endif
    }
    return res;
}