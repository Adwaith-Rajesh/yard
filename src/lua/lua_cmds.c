#include "lua/lua_cmds.h"

YardMasterCtx *mctx = NULL;

static Container *_create_container_from_luat(lua_State *L, int index) {
    switch (lua_type(L, index)) {
        case LUA_TSTRING:
            return create_str_container(lua_tostring(L, index), mctx->allocator, mctx->deallocator);

        case LUA_FLOAT_FLOAT:  // the define numbers are the same for INT
        case LUA_FLOAT_DOUBLE:
        case LUA_FLOAT_LONGDOUBLE:
            return create_float_container(lua_tonumber(L, index), mctx->allocator, mctx->deallocator);
        default:
            return NULL;
    }
}

static void _push_container_to_lua(lua_State *L, Container *container) {
    switch (container->type) {
        case INT:
            lua_pushinteger(L, container->data._int);
            return;
        case FLOAT:
            lua_pushnumber(L, (double)container->data._float);
            return;
        case STR:
            lua_pushstring(L, container->data._str);
            return;
        default:
            return;
    }
}

LUA_CFUNC(exit) {
    (void)L;
    if (mctx == NULL) {
        return 0;
    }

    mctx_free(mctx);
    return 0;
}

LUA_CFUNC(get) {
    SET_ARG_COUNT(1, "yard_get require 1 argument, key");

    Container *c = map_get(mctx->_default_map, lua_tostring(L, 1));
    if (c == NULL) {
        lua_pushnil(L);
        return 1;
    }

    _push_container_to_lua(L, c);
    return 1;
}

LUA_CFUNC(set) {
    SET_ARG_COUNT(2, "yard_set requires 2 arguments, key value");

    if (lua_type(L, 1) != LUA_TSTRING) {
        return luaL_error(L, "key must be a string");
    }

    Container *c = _create_container_from_luat(L, 2);

    if (c == NULL) {
        return luaL_error(L, "value must be int, float or string");
    }

    map_set(mctx->_default_map, lua_tostring(L, 1), c);
    return 0;
}

LUA_CFUNC(del) {
    SET_ARG_COUNT(1, "yard_del required 1 argument, keyname");

    if (lua_type(L, 1) != LUA_TSTRING) {
        return luaL_error(L, "key must be a  string");
    }

    map_delete(mctx->_default_map, lua_tostring(L, 1));
    return 0;
}

void register_functions(lua_State *L) {
    REGISTER_LUA_FUNC("yard_exit", exit);
    REGISTER_LUA_FUNC("yard_get", get);
    REGISTER_LUA_FUNC("yard_set", set);
    REGISTER_LUA_FUNC("yard_del", del);
}

int luaopen_yard(lua_State *L) {
    register_functions(L);

    mctx = mctx_create(malloc, free, realloc);

    return 0;
}
