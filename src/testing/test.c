#include "test.h"

void assertfail(char *file, int line)
{
    printf(" | " RED("Assert failed on ") "%s:%d", file, line);
}

void suiteinit(char *suite_name)
{
    printf(YELLOW("Running test suite: ") MAGENTA("%s\n"), suite_name);
}

void testinit(char *test_name)
{
    printf(CYAN("%s"), test_name);
}

void testfail()
{
    printf(BOLD_RED(" FAILED!"));
}

void testpass()
{
    printf(BOLD_GREEN(" PASSED\n")); 
}

extern void *twiddle;

int main(void)
{
    vector_testuite();
    matrix_testsuite();
    fft_testsuite();
    features_testsuite();
}