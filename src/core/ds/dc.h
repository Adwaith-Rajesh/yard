// data containers

#ifndef YARD_DS_DC_H
#define YARD_DS_DC_H

#include <stddef.h>

#include "core/cmd/parser.h"
#include "mctx.h"

// what type of data does a container hold
typedef enum {
    INT = 0,
    FLOAT,
    STR,
    CMD_FN_PTR
} container_t;

typedef void (*CmdFnType)(YardMasterCtx *, ParserCtx *, CmdResult *);

typedef struct {
    void *(*allocator)(size_t);
    void (*deallocator)(void *);

    container_t type;
    union {
        char *_str;
        int _int;
        float _float;
        CmdFnType _fn;
    } data;
} Container;

Container *create_int_container(int data, void *(allocator)(size_t), void (*deallocator)(void *));

Container *create_float_container(float data, void *(allocator)(size_t), void (*deallocator)(void *));

Container *create_str_container(const char *data, void *(allocator)(size_t), void (*deallocator)(void *));

Container *create_str_container(const char *data, void *(allocator)(size_t), void (*deallocator)(void *));

Container *create_cmd_fn_container(CmdFnType fn, void *(allocator)(size_t), void (*deallocator)(void *));

void container_free(Container *container);

void container_print(Container *container);

Container *container_clone(Container *container);

#endif
