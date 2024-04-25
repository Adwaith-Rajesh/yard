#include "core/ds/str.h"

#include <stdio.h>
#include <strings.h>

Str *str_create(void *(*allocator)(size_t),
                void (*deallocator)(void *),
                void *(*reallocator)(void *, size_t)) {
    Str *new_str = allocator(sizeof(Str));
    new_str->str = allocator(sizeof(char) * 2);
    new_str->_size = 2;
    new_str->len = 0;

    // init the string
    bzero(new_str->str, new_str->_size);

    new_str->allocator = allocator;
    new_str->deallocator = deallocator;
    new_str->reallocator = reallocator;

    return new_str;
}

void str_free(Str *str) {
    str->deallocator(str->str);
    str->deallocator(str);
}

void str_print(Str *str) {
    printf("%s", str->str);
}

size_t str_len(Str *str) {
    return str->len;
}

Str *str_append_char(Str *str, const char c) {
    // printf("str_len %zu\n", str->len);
    if (str->len == (str->_size - 1)) {
        // printf("in if\n");
        str->str = str->reallocator(str->str, str->_size * 2);

        size_t size_prev = str->_size;

        str->_size *= 2;

        // init the new allocated chunk
        bzero(&(str->str[size_prev]), str->_size - size_prev);
    };

    str->str[str->len++] = c;
    return str;
}

Str *str_append_charp(Str *str, const char *p) {
    int c;
    while ((c = *p++) != '\0') {
        str_append_char(str, c);
    }
    return str;
}
