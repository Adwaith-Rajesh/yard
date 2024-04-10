#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define STR(X) #X

// ...................  colors  ...................  //

#define C_RED "\x1b[31;1m"     // bold red
#define C_GREEN "\x1b[32;1m"   // bold green
#define C_YELLOW "\x1b[33;1m"  // bold yellow
#define C_BLUE "\x1b[34;1m"    // bold blue
#define C_RESET "\x1b[0m"

#define TEXT_RED(str) C_RED str C_RESET        // bold red
#define TEXT_GREEN(str) C_GREEN str C_RESET    // bold green
#define TEXT_YELLOW(str) C_YELLOW str C_RESET  // bold yellow
#define TEXT_BLUE(str) C_BLUE str C_RESET      // bold blue

// ................... idk ...................  //

int total_test_count = 0;
int total_failed = 0;
int curr_test_fail = 0;
struct winsize w;  // initialized during test_init

#define PRINT_SEP                                   \
    printf("\n" C_BLUE);                            \
    for (int i = 0; i < w.ws_col; i++) printf("="); \
    printf(C_RESET "\n")

// ................... TESTING MACROS ................... //

#define RUN_TEST(func)                                                                     \
    do {                                                                                   \
        total_test_count++;                                                                \
        curr_test_fail = 0;                                                                \
        func();                                                                            \
        if (curr_test_fail == 1) {                                                         \
            printf("(%s:%d) " TEXT_RED("%s - FAILED\n\n"), __FILE__, __LINE__, STR(func)); \
            total_failed++;                                                                \
        } else {                                                                           \
            printf("(%s:%d) " TEXT_GREEN("%s - PASSED\n"), __FILE__, __LINE__, STR(func)); \
        }                                                                                  \
    } while (0);

#define TEST_ASSERT(C)                                                            \
    do {                                                                          \
        if (!(C)) {                                                               \
            curr_test_fail = 1;                                                   \
            printf("\t(lno: %d): " STR(C) " " TEXT_RED("FAILED") "\n", __LINE__); \
        }                                                                         \
    } while (0)

#define TEST_ASSERT_MSG(C, MSG, ...)                                              \
    do {                                                                          \
        if (!(C)) {                                                               \
            curr_test_fail = 1;                                                   \
            printf("\t(lno: %d): " STR(C) " " TEXT_RED("FAILED") "\n", __LINE__); \
            printf(MSG);                                                          \
        }                                                                         \
    } while (0)

// test function implementation
// Usage:
/*
    #define TEST_IMPLEMENTATION
    #include "test.h"

    void test1() {
        TEST_ASSERT(1 == 2);
    }

    int main() {
        init_test();

        RUN_TEST(test1);

        print_test_statistic();
    }

*/
#ifdef TEST_IMPLEMENTATION

void init_test() {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
}

void print_test_statistic() {
    PRINT_SEP;

    if (total_failed > 0) {
        printf(C_RED "Test Failed ");
    } else {
        printf(C_GREEN "Test Passed ");
    }
    printf("(Total: %d Passed: %d Failed: %d)" C_RESET,
           total_test_count, total_test_count - total_failed, total_failed);

    PRINT_SEP;
}

int done_test() {
    if (total_failed > 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#endif
#endif
