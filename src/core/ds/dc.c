#include "dc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/utils/log.h"

Container *create_int_container(int data, void *(allocator)(size_t)) {
    Container *new_container = allocator(sizeof(Container));
    new_container->type = INT;
    new_container->data._int = data;
    return new_container;
}

Container *create_float_container(float data, void *(allocator)(size_t)) {
    Container *new_container = allocator(sizeof(Container));
    new_container->type = FLOAT;
    new_container->data._float = data;
    return new_container;
}

Container *create_str_container(char data[1024], void *(allocator)(size_t)) {
    if (strlen(data) > 1022) {
        LOG_WARN("create_str_container: data len exceeds the 1022 char limit. truncating data.");
        data[1023] = 0;
    }

    Container *new_container = allocator(sizeof(Container));
    new_container->type = STR;
    strncpy(new_container->data._str, data, 1024);
    return new_container;
}

void container_free(Container *container) {
    if (container == NULL) {
        return;
    }
    free(container);
}

void container_print(Container *container) {
    if (container == NULL) {
        return;
    }

    if (container->type == INT) {
        printf("Container{container_t = INT, data = %d}\n", container->data._int);
        return;
    }

    if (container->type == FLOAT) {
        printf("Container{container_t = FLOAT, data = %f}\n", container->data._float);
        return;
    }

    if (container->type == STR) {
        printf("Container{container_t = STR, data = %s}\n", container->data._str);
        return;
    }
}
