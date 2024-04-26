#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/ds/str.h"
#include "yard.h"

Str *get_cmd(Str *str) {
    int c;
    while ((c = getc(stdin))) {
        if (c == EOF || c == '\n') {
            break;
        }
        str_append_char(str, c);
    }
    return str;
}

int check_quit(Str *str) {
    return strncmp(str->str, "q", 1) == 0;
}

int main() {
    printf("%s", LICENSE);

    Str *str = str_create(malloc, free, realloc);

    while (1) {
        printf("yard >> ");
        get_cmd(str);

        if (check_quit(str)) {
            break;
        }

        str_print(str);
        printf("\n");
        str_clear(str);
    }

    str_free(str);
}
