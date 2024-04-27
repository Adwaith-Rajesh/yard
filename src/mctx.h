#ifndef YARD_MCTX_TYPE_H
#define YARD_MCTX_TYPE_H

#include "core/ds/list.h"
#include "core/ds/map.h"
#include "core/ds/str.h"

// stores all the maps, sets and list created dusting a session
typedef struct {
    void *(*allocator)(size_t);
    void (*deallocator)(void *);
    void *(*reallocator)(void *, size_t);

    Map *_commands;

    // map used to direct KV pairs
    // set key val
    // get key
    Map *_default_map;

    // map user to set user created maps
    // setz mapname key val
    // getz mapname key
    Map *_user_maps;

    // list used to directly add value to
    // pushl val
    // popr
    List *_default_list;

    // named lists created by users
    // pushlz list_name val
    // popr list_name
    Map *_user_list;  // Map[str, List]
} YardMasterCtx;

typedef enum {
    R_ERROR = 0,
    R_INT,
    R_FLOAT,
    R_STR,
    R_HELP
} result_t;

// store the result of a command
typedef struct {
    // 1 on success, 0 on error
    int ok;

    // type
    result_t result_type;

    // used to store error messages or help messages
    // or anything like that
    Str *emsg;

    // all the types that we support rn
    Str *_str;     // store the string result
    int _int;      // store the int result
    float _float;  // store the float result
} CmdResult;

#endif
