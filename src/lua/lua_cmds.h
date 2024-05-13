#ifndef YARD_LUA_CMDS_H
#define YARD_LUA_CMDS_H

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>

#include "yard.h"

#define LUA_FUNC_NAME(FN) lua_yard_##FN
#define LUA_EXPORT_FUNC_NAME(FN) yard_##FN

#define LUA_CFUNC(FN) static int LUA_FUNC_NAME(FN)(lua_State * L)

#define REGISTER_LUA_FUNC(NAME, FN) lua_register(L, NAME, LUA_FUNC_NAME(FN))

// utils

#define SET_ARG_COUNT(N, ERR_MSG)      \
    if (lua_gettop(L) != N) {          \
        return luaL_error(L, ERR_MSG); \
    }

#endif
