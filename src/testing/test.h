#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h>
#include <stdlib.h>

#define TEST_RES_DIR(string) "./results/tests/" string

#define COLOR_RESET          "\x1b[0m"
#define YELLOW(string)       "\x1b[33m" string COLOR_RESET
#define MAGENTA(string)      "\x1b[35m" string COLOR_RESET
#define CYAN(string)         "\x1b[36m" string COLOR_RESET
#define RED(string)          "\033[31m" string COLOR_RESET
#define BOLD_GREEN(string)   "\033[1m\033[32m" string COLOR_RESET
#define BOLD_RED(string)     "\033[1m\033[31m" string COLOR_RESET

#define ASSERT(expr) \
    if (!(expr)) { \
        testfail(); \
        assertfail(__FILE__, __LINE__); \
        printf(" | Expected: " #expr "\n"); \
        exit(1); \
    }

#define ASSERT_EQ(X,Y) \
    if (!(X == Y)) {\
        testfail(); \
        assertfail(__FILE__, __LINE__); \
        printf(" | Expected: " #X RED(" == ") #Y "\n"); \
        exit(1); \
    }

void assertfail(char *file, int line);

void suiteinit(char *suite_name);

void testinit(char *test_name);

void testfail();

void testpass();

/* Test suites */
void vector_testuite();
void matrix_testsuite();
void fft_testsuite();
void features_testsuite();
void dtw_testsuite();

#endif /* __TEST_H__ */