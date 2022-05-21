#include "test.h"
#include "../vector.h"
#include "math.h"

void vector_vectorf_dot_asm()
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

void vector_vectorf_mul_r_asm()
{
    testinit("vectorf_mul_r_asm");

    float v1[8] = {2, 2, 2, 2, 2, 2, 2, 2};
    float v2[8] = {2, 2, 2, 2, 2, 2, 2, 2};

    for (unsigned int i = 0; i < 8; i++) {
        vectorf_mul_r_asm(v1, v2, i);
    }
    for (unsigned int j = 0; j < 8; j++) {
        ASSERT_EQ(v1[j], 2<<(7-j));
    }

    testpass();
}

void vector_vectorf_smul_r_asm()
{
    testinit("vectorf_smul_r_asm");

    float v[8] = {2, 2, 2, 2, 2, 2, 2, 2};
    for (unsigned int i = 0; i < 8; i++) {
        vectorf_smul_r_asm(v, 2, i);
    }
    for (unsigned int j = 0; j < 8; j++) {
        ASSERT_EQ(v[j], 2<<(7-j));
    }

    testpass();
}

void vector_vectorf_norm_asm()
{
    testinit("vectorf_norm_asm");

    float v[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    for (unsigned int i = 0; i < 8; i++) {
        float norm = vectorf_norm_asm(v, i);
        ASSERT_EQ(norm, sqrtf(i));
    }
    testpass();
}

void vector_testuite()
{
    suiteinit("Vector");
    vector_vectorf_dot_asm();
    vector_vectorf_mul_r_asm();
    vector_vectorf_smul_r_asm();
    vector_vectorf_norm_asm();
}