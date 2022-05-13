#include "test.h"
#include "../matrix.h"

void matrix_vectorf_dot_asm()
{
    testinit("vectorf_dot_asm");

    float v1[8] = {2, 2, 2, 2, 2, 2, 2, 2};
    float v2[8] = {2, 2, 2, 2, 2, 2, 2, 2};

    for (unsigned int i = 0; i < 8; i++) {
        float result = vectorf_dot_asm(v1, v2, i);
        ASSERT_EQ(result, 4 * i);
    }

    testpass();
}

void matrix_matrixf_dot_fast()
{
    testinit("matrixf_dot_fast");
    /* Square Multiplication */
    matrixf *m1 = matrixf_new(3, 3, ROW_MAJOR);
    matrixf *m2 = matrixf_new(3, 3, COL_MAJOR);
    /**
     * [1, 1, 1] * [1, 2, 3] = [ 3,  6,  9]
     * [2, 2, 2]   [1, 2, 3]   [ 6, 12, 18]
     * [3, 3, 3]   [1, 2, 3]   [ 9, 18, 27]
     */
    float arr1[9] = {1, 1, 1,
                     2, 2, 2,
                     3, 3, 3};
    float arr2[9] = {1, 1, 1,
                     2, 2, 2,
                     3, 3, 3};
    float expected[9] = {3, 6, 9,
                         6, 12, 18,
                         9, 18, 27};
    m1->data = arr1;
    m2->data = arr2;

    matrixf *result;
    for (unsigned int m2_ncols = 1; m2_ncols < 4; m2_ncols++) {
        m2->cols = m2_ncols;
        result = matrixf_dot_fast(m1, m2);
            ASSERT(result->rows == 3);
            ASSERT(result->cols == m2_ncols);
            for (unsigned int i = 0; i < result->rows; i++) {
                for (unsigned int j = 0; j < result->cols; j++) {
                    ASSERT_EQ(*matrixf_at(result, i, j), expected[i * 3 + j]);
                }
            }
    }
    matrixf_free(result);
    free(m1);
    free(m2);
    testpass();
}

void matrix_testsuite()
{
    suiteinit("Matrix");
    matrix_vectorf_dot_asm();
    matrix_matrixf_dot_fast();
}