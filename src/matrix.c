#include "matrix.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** 
 * Float Matrix
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
            float scalar = 0;
            #ifdef __MATRIX_SSE__

            scalar = vectorf_dot_asm(m1_row_i, m2_col_j, m1->cols);

            #else

            for (unsigned int k = 0; k < m1->cols; k++) {
                scalar += m1_row_i[k] * m2_col_j[k];
            }

            #endif

            *matrixf_at(res, i, j) = scalar;
        }
    }
    return res;
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

float vectorf_dist(float *v1, float *v2, unsigned int length)
{
    float sum = 0;
    for (unsigned int i = 0; i < length; i++) {
        sum += powf(v1[i] - v2[i], 2);
    }
    return sqrtf(sum);
}

void matrixf_print(matrixf *mat)
{
    for (unsigned int i = 0; i < mat->rows; i++) {
        printf("|");
        for (unsigned int j = 0; j < mat->cols; j++) {
            printf(" %.0f ", *matrixf_at(mat, i, j));
        }
        printf("|\n");
    }
}

/**
 * Float complex matrix 
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

void matrixfc_reshape(matrixfc *mat, unsigned int rows, unsigned int cols)
{
    /* Check new shape is within bounds*/
    if (rows > mat->rows || cols > mat->cols) {
        exit(-1);
    }
    if (mat->order == ROW_MAJOR) {
        for (unsigned int i = 0; i < rows; i++) {
            fcomplex *row_i = matrixfc_at(mat, i, 0);
            memmove(mat->data + (i * cols), row_i, sizeof(fcomplex) * cols);
        }
    } else {
        for (unsigned int j = 0; j < cols; j++) {
            fcomplex *col_i = matrixfc_at(mat, j, 0);
            memmove(mat->data + (j * rows), col_i, sizeof(fcomplex) * rows);
        }
    }

    fcomplex *data_r = realloc(mat->data, sizeof(fcomplex) * rows * cols);
    if(data_r) mat->data = data_r;

    mat->rows = rows;
    mat->cols = cols;
}

matrixf *matrixfc_abs(matrixfc *mat)
{
    matrixf *res = matrixf_new(mat->rows, mat->cols, ROW_MAJOR);

    for (unsigned int i = 0; i < mat->rows; i++) {
        fcomplex *mat_row_i = matrixfc_at(mat, i, 0);
        float *res_row_i = matrixf_at(res, i, 0);
        for (unsigned int j = 0; j < mat->cols; j++) {
            res_row_i[j] = fcabs(mat_row_i[j]);
        }
    }
    return res;
}

void matrixfc_print(matrixfc *mat)
{
    for (unsigned int i = 0; i < mat->rows; i++) {
        printf("|");
        for (unsigned int j = 0; j < mat->cols; j++) {
            fcomplex curr_entry = *matrixfc_at(mat, i, j);
            printf("(%.0f,%.0f)", curr_entry.real, curr_entry.imag);
        }
        printf("|\n");
    }
}