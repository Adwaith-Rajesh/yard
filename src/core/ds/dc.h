// data containers

#ifndef YARD_DS_DC_H
#define YARD_DS_DC_H

#include <stddef.h>

// what type of data does a container hold
typedef enum {
    INT = 0,
    FLOAT,
    STR
} container_t;

typedef struct {
    void *(*allocator)(size_t);
    void (*deallocator)(void *);

    container_t type;
    union {
        char *_str;
        int _int;
        float _float;
    } data;
} Container;

Container *create_int_container(int data, void *(allocator)(size_t), void (*deallocator)(void *));

Container *create_float_container(float data, void *(allocator)(size_t), void (*deallocator)(void *));

Container *create_str_container(const char *data, void *(allocator)(size_t), void (*deallocator)(void *));

void container_free(Container *container);

void container_print(Container *container);

#endif
