// just a bunch of repetitive checks

#ifndef YARD_UTILS_CHECK_H
#define YARD_UTILS_CHECK_H

#include <stdio.h>
#include <stdlib.h>

// check whether X is null if null exec B and exit
#define CHECK_NULL_EXIT(X, B) \
    if ((X) == NULL) {        \
        B;                    \
        exit(EXIT_FAILURE);   \
    }

// if the C is true exec B and exit
#define CHECK_COND_EXIT(C, B) \
    if (C) {                  \
        B;                    \
        exit(EXIT_FAILURE);   \
    }

// if C is true exit after printing perror
#define CHECK_WITH_PERROR(C, msg) \
    if ((C)) {                    \
        perror(msg);              \
        exit(EXIT_FAILURE);       \
    }

#endif
