#ifndef YARD_DS_STR_H
#define YARD_DS_STR_H

#include <stddef.h>

typedef struct {
    char *str;
    size_t len;
    size_t _size;

    void *(*allocator)(size_t);
    void (*deallocator)(void *);
    void *(*reallocator)(void *, size_t);
} Str;

// create an empty Str
// by default allocated size with 16
Str *str_create(void *(*allocator)(size_t), void (*deallocator)(void *), void *(*reallocator)(void *, size_t));

// create a string from char *
Str *str_append_charp(Str *str, const char *p);

// add a character to the given string
Str *str_append_char(Str *str, const char c);

// print a given Str
void str_print(Str *str);

// get the length of the string
size_t str_len(Str *str);

// free the string
void str_free(Str *str);

#endif
