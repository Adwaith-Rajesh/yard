#include "dc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "core/utils/log.h"

Container *create_int_container(int data, void *(allocator)(size_t), void (*deallocator)(void *)) {
    Container *new_container = allocator(sizeof(Container));
    new_container->allocator = allocator;
    new_container->deallocator = deallocator;
    new_container->type = INT;
    new_container->data._int = data;
    return new_container;
}

Container *create_float_container(float data, void *(allocator)(size_t), void (*deallocator)(void *)) {
    Container *new_container = allocator(sizeof(Container));
    new_container->allocator = allocator;
    new_container->deallocator = deallocator;
    new_container->type = FLOAT;
    new_container->data._float = data;
    return new_container;
}

Container *create_str_container(const char *data, void *(allocator)(size_t), void (*deallocator)(void *)) {
    Container *new_container = allocator(sizeof(Container));
    new_container->allocator = allocator;
    new_container->deallocator = deallocator;
    new_container->type = STR;
    size_t data_len = strlen(data);
    new_container->data._str = allocator(data_len + 1);
    bzero(new_container->data._str, data_len + 1);
    strncpy(new_container->data._str, data, data_len + 1);
    return new_container;
}

Container *create_cmd_fn_container(CmdFnType fn, void *(allocator)(size_t), void (*deallocator)(void *)) {
    Container *new_container = allocator(sizeof(Container));
    new_container->allocator = allocator;
    new_container->deallocator = deallocator;
    new_container->type = CMD_FN_PTR;
    new_container->data._fn = fn;
    return new_container;
}

void container_free(Container *container) {
    if (container == NULL) {
        return;
    }

    if (container->type == STR) {
        container->deallocator(container->data._str);
    }

    container->deallocator(container);
}

void container_print(Container *container) {
    switch (container->type) {
        case INT:
            printf("Container{container_t = INT, data = %d}\n", container->data._int);
            return;
        case FLOAT:
            printf("Container{container_t = FLOAT, data = %f}\n", container->data._float);
            return;
        case STR:
            printf("Container{container_t = STR, data = %s}\n", container->data._str);
            return;
        case CMD_FN_PTR:
            printf("Container{container_t = CMD_FN_PTR, data = %p}\n", container->data._fn);
            return;
    }
}
