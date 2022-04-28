#include "matrix.h"
#include <stdlib.h>

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