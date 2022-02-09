#include "test.h"
#include "../fcomplex.h"

void fcomplex_fcdot()
{
    testinit("fcomplex_fcdot");
    unsigned int len = 4;
    fcomplex *x = (fcomplex*)calloc(len, sizeof(fcomplex));
    fcomplex *y = (fcomplex*)calloc(len, sizeof(fcomplex));

    for (unsigned int i = 0; i < len; i++) {
        x[i] = (fcomplex){1, 1};
        y[i] = (fcomplex){1, 1};
    }

    /* ----------fcdot----------*/ 
    fcomplex result = fcdot(x, y, len);
    fcomplex expected = (fcomplex){0, 8};
    ASSERT_EQ(result.real, expected.real);
    ASSERT_EQ(result.imag, expected.imag);

    /* ----------fcdot_asm----------*/ 
    /* Even length */
    result = fcdot_asm(x, y, len);
    ASSERT_EQ(result.real, expected.real);
    ASSERT_EQ(result.imag, expected.imag);

    /* Odd length */
    result = fcdot_asm(x, y, len-1);
    expected = (fcomplex){0, 6};
    ASSERT_EQ(result.real, expected.real);
    ASSERT_EQ(result.imag, expected.imag);

    free(x);
    free(y);
    testpass();
}

void fcomplex_testsuite()
{
    suiteinit("FCOMPLEX");
    fcomplex_fcdot();
}