#include "test.h"
#include "../matrix.h"

void matrix_vectorfc_dot_asm()
{
    testinit("vectorf_dot_asm");
    unsigned int len = 16;
    fcomplex *x = (fcomplex*)calloc(len, sizeof(fcomplex));
    fcomplex *y = (fcomplex*)calloc(len, sizeof(fcomplex));

    for (unsigned int i = 0; i < len; i++) {
        x[i] = (fcomplex){2, 0};
        y[i] = (fcomplex){2, 0};
    }

    for (unsigned int i = 1; i <= len; i++) {
        fcomplex result = vectorfc_dot_asm(x, y, i);
        fcomplex expected = (fcomplex){4 * i, 0};
        ASSERT_EQ(result.real, expected.real);
        ASSERT_EQ(result.imag, expected.imag);
    }

    free(x);
    free(y);

    testpass();
}

void matrix_matrixfc_dot_fast()
{
    testinit("matrixf_dot_fast");

    matrixfc *m1 = matrixfc_new(2, 2, ROW_MAJOR);
    matrixfc *m2 = matrixfc_new(2, 2, COL_MAJOR);
    /**
     * [1, 1] * [1, 2] = [2, 4]
     * [2, 2]   [1, 2]   [4, 8]
     */
    fcomplex arr1[4] = {{1,0}, {1,0},
                        {2,0}, {2,0}};
    fcomplex arr2[4] = {{1,0}, {1,0},
                        {2,0}, {2,0}};
    fcomplex expected[4] = {{2,0}, {4,0},
                            {4,0}, {8,0}};
    m1->data = arr1;
    m2->data = arr2;

    matrixfc *result = matrixfc_dot_fast(m1, m2);
    ASSERT(result->rows == 2);
    ASSERT(result->cols == 2);

    for (unsigned int i = 0; i < 4; i++) {
        ASSERT_EQ(result->data[i].real, expected[i].real);
        ASSERT_EQ(result->data[i].imag, expected[i].imag);
    }

    free(m1);
    free(m2);
    matrixfc_free(result);
    testpass();
}

void matrix_testsuite()
{
    suiteinit("Matrix");
    matrix_vectorfc_dot_asm();
    matrix_matrixfc_dot_fast();
}