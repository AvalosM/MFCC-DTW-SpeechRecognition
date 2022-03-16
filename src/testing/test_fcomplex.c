#include "test.h"
#include "../fcomplex.h"

void fcomplex_fcfdot()
{
    testinit("fcomplex_fcfdot");
    unsigned int len = 4;
    fcomplex *x = (fcomplex*)calloc(len, sizeof(fcomplex));
    float *y = (float*)calloc(len, sizeof(float));

    for (unsigned int i = 0; i < len; i++) {
        x[i] = (fcomplex){1, 2};
        y[i] = 2;
    }

    /* ----------fcfdot----------*/ 
    fcomplex result = fcfdot(x, y, len);
    fcomplex expected = (fcomplex){8, 16};
    ASSERT_EQ(result.real, expected.real);
    ASSERT_EQ(result.imag, expected.imag);

    /* ----------fcfdot_asm----------*/ 
    /* Even length */
    result = fcfdot_asm(x, y, len);
    ASSERT_EQ(result.real, expected.real);
    ASSERT_EQ(result.imag, expected.imag);

    /* Odd length */
    result = fcfdot_asm(x, y, len-1);
    expected = (fcomplex){6, 12};
    ASSERT_EQ(result.real, expected.real);
    ASSERT_EQ(result.imag, expected.imag);

    free(x);
    free(y);
    testpass();
}

void fcomplex_testsuite()
{
    suiteinit("FCOMPLEX");
    fcomplex_fcfdot();
}