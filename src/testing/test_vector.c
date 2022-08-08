#include "test.h"
#include "../vector.h"
#include "math.h"


void vector_vectorf_dot_asm()
{
    testinit("vectorf_dot_asm");

    float v1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float v2[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    float expected = 0;
    for (unsigned int i = 0; i < 10; i++) {
        float result = vectorf_dot_asm(v1, v2, i);
        ASSERT(result - expected < EPSILON);
        expected += i + 1;
    }

    testpass();
}

void vector_vectorf_mul_asm()
{
    testinit("vectorf_mul_asm");

    float v1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float v2[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float result[10] = {1, 4, 9, 16, 25, 36, 49, 81, 100};

    for (unsigned int i = 0; i < 10; i++) {
        vectorf_mul_asm(v1, v2, result, i);
        for (unsigned int j = 0; j < i; j++) {
            ASSERT_EQ(result[j], powf(v1[j], 2));
        }
    }
    
    testpass();
}

void vector_vectorf_add_asm()
{
    testinit("vectorf_add_asm");

    float v1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float v2[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float result[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (unsigned int i = 0; i < 10; i++) {
        vectorf_add_asm(v1, v2, result, i);
        for (unsigned int j = 0; j < i; j++) {
            ASSERT_EQ(result[j], (j + 1)*2);
        }
    }

    testpass();
}

void vector_vectorf_sub_asm()
{
    testinit("vectorf_sub_asm");

    float v1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float v2[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float result[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 10; i++) {
        vectorf_sub_asm(v1, v2, result, i);
        for (int j = 0; j < i; j++) {
            ASSERT_EQ(result[j], -(j+1));
        }
    }

    testpass();
}


void vector_vectorf_smul_asm()
{
    testinit("vectorf_smul_asm");

    float v[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float result[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (unsigned int i = 0; i < 10; i++) {
        vectorf_smul_asm(v, 2, result, i);
        for (unsigned int j = 0; j < i; j++) {
            ASSERT_EQ(result[j], v[j] * 2);
        }
    }

    testpass();
}

void vector_vectorfc_mul_asm()
{
    testinit("vectorfc_mul_asm");

    fcomplex v1[6] = {{0,0}, {1,1}, {0,1}, {1,0}, {0,1}, {-1,-1}};
    fcomplex v2[6] = {{1,1}, {1,1}, {1,1}, {1,1}, {0,1}, {1,1}};
    fcomplex result[6] = {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
    fcomplex expected[6] = {{0,0}, {0,2}, {-1,1}, {1,1}, {-1,0}, {0,-2}};

    for (unsigned int i = 0; i <= 6; i++) {
        vectorfc_mul_asm(v1, v2, result, i);
        for (unsigned int j = 0; j < i; j++) {
            ASSERT(fcequ(result[j], expected[j], EPSILON));
        }
    }
    
    testpass();
}

void vector_vectorfc_add_asm()
{
    testinit("vectorfc_add_asm");

    fcomplex v1[6] = {{0,0}, {1,1}, {0,1}, {1,0}, {0,1}, {-1,-1}};
    fcomplex v2[6] = {{1,1}, {1,1}, {1,1}, {1,1}, {0,1}, {1,1}};
    fcomplex result[6] = {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
    fcomplex expected[6] = {{1,1}, {2,2}, {1,2}, {2,1}, {0,2}, {0,0}};

    for (unsigned int i = 0; i <= 6; i++) {
        vectorfc_add_asm(v1, v2, result, i);
        for (unsigned int j = 0; j < i; j++) {
            ASSERT(fcequ(result[j], expected[j], EPSILON));
        }
    }

    testpass();
}

void vector_vectorfc_sub_asm()
{
    testinit("vectorfc_sub_asm");

    fcomplex v1[6] = {{0,0}, {1,1}, {0,1}, {1,0}, {0,1}, {-1,-1}};
    fcomplex v2[6] = {{1,1}, {1,1}, {1,1}, {1,1}, {0,1}, {1,1}};
    fcomplex result[6] = {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
    fcomplex expected[6] = {{-1,-1}, {0,0}, {-1,0}, {0,-1}, {0,0}, {-2,-2}};

    for (unsigned int i = 0; i <= 6; i++) {
        vectorfc_sub_asm(v1, v2, result, i);
        for (unsigned int j = 0; j < i; j++) {
            ASSERT(fcequ(result[j], expected[j], EPSILON));
        }
    }

    testpass();
}

void vector_testuite()
{
    suiteinit("Vector");
    vector_vectorf_dot_asm();
    vector_vectorf_mul_asm();
    vector_vectorf_add_asm();
    vector_vectorf_sub_asm();
    vector_vectorf_smul_asm();
    vector_vectorfc_mul_asm();
    vector_vectorfc_add_asm();
    vector_vectorfc_sub_asm();
}