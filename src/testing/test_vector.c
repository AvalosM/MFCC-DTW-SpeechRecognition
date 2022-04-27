#include "test.h"
#include "../vector.h"

void vector_fcomplex_dot()
{
    testinit("vector_fcomplex_dot");
    unsigned int len = 8;
    fcomplex *x = (fcomplex*)calloc(len, sizeof(fcomplex));
    fcomplex *y = (fcomplex*)calloc(len, sizeof(fcomplex));

    for (unsigned int i = 0; i < len; i++) {
        x[i] = (fcomplex){1, 1};
        y[i] = (fcomplex){1, 1};
    }

    for (unsigned int i = 1; i <= len; i++) {
        fcomplex result = fcomplex_vector_dot(x, y, i);
        fcomplex expected = (fcomplex){0, 2 * i};
        ASSERT_EQ(result.real, expected.real);
        ASSERT_EQ(result.imag, expected.imag);
    }
    free(x);
    free(y);

    testpass();
}

void vector_fcomplex_mul()
{
    testinit("vector_fcomplex_mul");
    unsigned int len = 8;
    fcomplex *x = (fcomplex*)calloc(len, sizeof(fcomplex));
    fcomplex *y = (fcomplex*)calloc(len, sizeof(fcomplex));

    for (unsigned int i = 0; i < len; i++) {
        x[i] = (fcomplex){1, 1};
        y[i] = (fcomplex){1, 1};
    }

    for (unsigned int i = 1; i <= len; i++) {
        fcomplex_vector_mul(x, y, i);
        fcomplex expected = (fcomplex){0, 2};
        for (unsigned int j = 0; j < i; j++) {
            ASSERT_EQ(x[j].real, expected.real);
            ASSERT_EQ(x[j].imag, expected.imag);
            x[j] = (fcomplex){1, 1};
        }
    }
    free(x);
    free(y);

    testpass();
}
void vector_float_mul()
{
    testinit("vector_float_mul");
    unsigned int len = 8;
    float *x = (float*)calloc(len, sizeof(float));
    float *y = (float*)calloc(len, sizeof(float));

    for (unsigned int i = 0; i < len; i++) {
        x[i] = 2;
        y[i] = 2;
    }

    for (unsigned int i = 1; i <= len; i++) {
        float_vector_mul(x, y, i);
        float expected = 4;
        for (unsigned int j = 0; j < i; j++) {
            ASSERT_EQ(x[j], expected);
            x[j] /= 2;
        }
    }
    free(x);
    free(y);

    testpass();
}

void vector_testsuite()
{
    suiteinit("Vector");
    vector_fcomplex_dot();
    vector_fcomplex_mul();
    vector_float_mul();
}