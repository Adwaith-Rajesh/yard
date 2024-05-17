#include "core/cmd/cmds.h"

#include <stdio.h>

#include "core/cmd/parser.h"
#include "mctx.h"

// static void CMD(YardMasterCtx *mctx, ParserCtx *pctx, )

// get keyname

DEFINE_CMD(get) {
    INIT_ARGS(ARG(keyname));
    Container *val = map_get(MCTX->_default_map, keyname->data._str);

    if (val == NULL) {
        SET_ERROR("key '", C_STR(keyname), "' does not exist");
        return;
    }

    gen_result(val, RES);
}

CMD_WRAP(get) {
    // CHECK_HELP("Usage:\n\tget keyname");
    CREATE_HELP(
        USAGE("get key"),
        DESC("get value associated with the key"),
        ARG_DESC("key", "the name of the key", "string"));

    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("get requires 1 argument, key", USE_HELP("get"));
    });
    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, "keyname must be a string");
    });

    EXE_CMD(get);
}

// end get

// set keyname value

DEFINE_CMD(set) {
    INIT_ARGS(ARG(keyname); ARG(val));
    map_set(MCTX->_default_map, keyname->data._str, container_clone(val));
    SET_RESULT_MSG("Done");
}

CMD_WRAP(set) {
    CREATE_HELP(
        USAGE("set key value"),
        DESC("sets a key value pair"),
        ARG_DESC("key", "the key name", "string"),
        ARG_DESC("value", "the value associated with the key", "any"));
    ENFORCE_ARG_COUNT(2, {
        SET_ERROR("set requires 2 arguments, key and val", USE_HELP("set"));
    });

    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, "key must be a string");
        TYPE_OF(2, ANY, "");
    });

    EXE_CMD(set);
}

// end set

// del keyname

DEFINE_CMD(del) {
    INIT_ARGS(ARG(keyname););

    if (map_exists(MCTX->_default_map, keyname->data._str) == 0) {
        SET_ERROR("key '", C_STR(keyname), "' does not exist");
        return;
    }

    map_delete(MCTX->_default_map, keyname->data._str);
    SET_RESULT_MSG("Done");
}

CMD_WRAP(del) {
    CREATE_HELP(
        USAGE("delete key"),
        DESC("delete key from the map"),
        ARG_DESC("key", "the key name", "string"));
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("del requires 1 argument, key", USE_HELP("del"));
    });
    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, "key must be a string");
    });
    EXE_CMD(del);
}

// end del

// help command
// help <- return all the available command
// help command <- returns the help of the given command

#define GET_HELP_CALL(CMD) (((Container *)map_get(MCTX->_commands, CMD))->data._fn)(MCTX, NULL, RES)

static void _get_all_cmds(MapEntry *entry, void *arg) {
    if (entry == NULL) {
        return;
    }

    CmdResult *res = arg;
    str_append_char(res->emsg, '\t');
    str_append_charp(res->emsg, entry->key);
    str_append_char(res->emsg, '\n');
}

DEFINE_CMD(help) {
    size_t arg_count = GET_ARG_COUNT();

    if (arg_count > 1) {
        SET_ERROR("help can only accept 0 or 1 argment");
        return;
    }

    if (arg_count == 0) {
        RES->result_type = R_HELP;
        str_append_charp(RES->emsg, "Available commands:\n");
        map_for_each(MCTX->_commands, _get_all_cmds, RES);
        return;
    }

    INIT_ARGS(ARG(command_name););

    if (map_exists(MCTX->_commands, command_name->data._str) == 0) {
        SET_ERROR("command '", C_STR(command_name), "' does not exist");
        return;
    }

    // call the command with mctx = NULL, pctx = NULL
    GET_HELP_CALL(command_name->data._str);
}

CMD_WRAP(help) {
    CHECK_HELP("Usage:\n\thelp\n\thelp command");
    EXE_CMD(help);
}

// pushl value

DEFINE_CMD(pushl) {
    INIT_ARGS(ARG(value););
    list_pushl(MCTX->_default_list, container_clone(value));
    SET_RESULT_MSG("Done");
}

CMD_WRAP(pushl) {
    CREATE_HELP(
        USAGE("pushl val"),
        DESC("push a value to the left of the list"),
        ARG_DESC("val", "the val to be pushed", "any"));
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("pushl requires one argument", USE_HELP("pushl"));
    });

    // arg type is not an issue
    NO_ARG_TYPE;

    EXE_CMD(pushl);
}

// end pushl

// popl

DEFINE_CMD(popl) {
    // popl does not accept args -> PCTX is not used
    UN_USED(PCTX);

    void *data = list_popl(MCTX->_default_list);
    if (data == NULL) {
        SET_RESULT_MSG("list is empty");
        return;
    }

    gen_result((Container *)data, RES);
    container_free(data);
}

CMD_WRAP(popl) {
    CREATE_HELP(
        USAGE("popl"),
        DESC("pop a value from the left of the list"),
        NULL);
    ENFORCE_ARG_COUNT(0, {
        SET_ERROR("popl does not accept any args", USE_HELP("popl"));
    });

    NO_ARG_TYPE;
    EXE_CMD(popl);
}
// end popl

// pushr value

DEFINE_CMD(pushr) {
    INIT_ARGS(ARG(value););
    list_pushr(MCTX->_default_list, container_clone(value));
    SET_RESULT_MSG("Done");
}

CMD_WRAP(pushr) {
    CHECK_HELP("Usage:\n\tpushr val\nPush a value to the right of the list.");
    CREATE_HELP(
        USAGE("pushr val"),
        DESC("push a value to the right of the list"),
        ARG_DESC("val", "the value to push", "any"));
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("pushr requires one argument", USE_HELP("pushr"));
    });

    NO_ARG_TYPE;
    EXE_CMD(pushr);
}

// end pushr

// popr

DEFINE_CMD(popr) {
    // pops does not accept any args -> PCTX is not used
    UN_USED(pctx);

    void *data = list_popr(MCTX->_default_list);
    if (data == NULL) {
        SET_RESULT_MSG("list is empty");
        return;
    }

    gen_result((Container *)data, RES);
    container_free(data);
}

CMD_WRAP(popr) {
    CREATE_HELP(
        USAGE("popr"),
        DESC("pop a value from the right of the list"),
        NULL);
    ENFORCE_ARG_COUNT(0, {
        SET_ERROR("popr does not accept any args", USE_HELP("popr"));
    });

    NO_ARG_TYPE;
    EXE_CMD(popr);
}
// end popl

// ################## custom list and maps ##################

// create

DEFINE_CMD(create) {
    INIT_ARGS(ARG(type); ARG(name););

    if (strncmp(type->data._str, "list", 4) != 0 && strncmp(type->data._str, "map", 3) != 0) {
        SET_ERROR("type must be list or map");
        return;
    }

    if (strncmp(type->data._str, "list", 4) == 0) {
        map_set(MCTX->_user_list, name->data._str, LIST_FROM_MCTX(MCTX));
        SET_RESULT_MSG("Done");
        return;
    }

    if (strncmp(type->data._str, "map", 3) == 0) {
        map_set(MCTX->_user_maps, name->data._str, MAP_FROM_MCTX(MCTX));
        SET_RESULT_MSG("Done");
    }
}

CMD_WRAP(create) {
    CREATE_HELP(
        USAGE("create type name"),
        DESC("creates a new list/map other than the default one"),
        ARG_DESC("type", "list or map", "string"),
        ARG_DESC("name", "the name of the new list/map", "string"));

    ENFORCE_ARG_COUNT(2, {
        SET_ERROR("create requires 2 argument.", USE_HELP("create"));
    });

    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, "create type name must be a string");
        TYPE_OF(2, STR, "name of the type must be a string");
    });

    EXE_CMD(create);
}

// end create

DEFINE_CMD(delete) {
    INIT_ARGS(ARG(type); ARG(name););

    if (strncmp(type->data._str, "list", 4) != 0 && strncmp(type->data._str, "map", 3) != 0) {
        SET_ERROR("type must be list or map");
        return;
    }

    if (strncmp(type->data._str, "list", 4) == 0) {
        if (map_exists(MCTX->_user_list, name->data._str) != 1) {
            SET_ERROR("list '", C_STR(name), "' does not exists");
            return;
        }
        map_delete(MCTX->_user_list, name->data._str);
        SET_RESULT_MSG("Done");
        return;
    }

    if (strncmp(type->data._str, "map", 3) == 0) {
        if (map_exists(MCTX->_user_maps, name->data._str) != 1) {
            SET_ERROR("map does not exists");
            return;
        }
        map_delete(MCTX->_user_maps, name->data._str);
        SET_RESULT_MSG("Done");
        return;
    }
}

CMD_WRAP(delete) {
    CREATE_HELP(
        USAGE("delete type name"),
        DESC("delete a user created list or map "),
        ARG_DESC("type", "list or map", "string"),
        ARG_DESC("name", "the name of the list/map", "string"));

    ENFORCE_ARG_COUNT(2, {
        SET_ERROR("create requires 2 arguments.", USE_HELP("delete"));
    });

    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, "create type name must be a string");
        TYPE_OF(2, STR, "name of the type must be a string");
    });

    EXE_CMD(delete);
}

// the Z commands

// getz

DEFINE_CMD(getz) {
    INIT_ARGS(ARG(mapname); ARG(key));

    if (map_exists(MCTX->_user_maps, mapname->data._str) == 0) {
        SET_ERROR("map '", C_STR(mapname), "' does not exist");
        return;
    }

    Map *user_map = map_get(MCTX->_user_maps, mapname->data._str);

    void *val = map_get(user_map, key->data._str);
    if (val == NULL) {
        SET_ERROR("key '", C_STR(key), "' does not exist");
        return;
    }

    gen_result((Container *)val, RES);
}

CMD_WRAP(getz) {
    CREATE_HELP(
        USAGE("getz map key"),
        DESC("get a value from a user defined map"),
        ARG_DESC("map", "the name of the map", "string"),
        ARG_DESC("key", "the name of the key", "string"));

    ENFORCE_ARG_COUNT(2, {
        SET_ERROR("getz requires 2 arguments", USE_HELP("getz"));
    });

    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, "map name must be a string");
        TYPE_OF(2, STR, "keyname must be a string");
    });

    EXE_CMD(getz);
}

// end getz

// setz

DEFINE_CMD(setz) {
    INIT_ARGS(ARG(mapname); ARG(key); ARG(value));

    if (map_exists(MCTX->_user_maps, mapname->data._str) == 0) {
        SET_ERROR("map '", C_STR(mapname), "' does not exist");
        return;
    }

    Map *user_map = map_get(MCTX->_user_maps, mapname->data._str);
    map_set(user_map, key->data._str, container_clone(value));
    SET_RESULT_MSG("Done");
}

CMD_WRAP(setz) {
    CREATE_HELP(
        USAGE("setz map key"),
        DESC("set a value in a user defined map"),
        ARG_DESC("map", "the name of the map", "string"),
        ARG_DESC("key", "the name of the key", "string"),
        ARG_DESC("value", "the value to set for the key", "string"));

    ENFORCE_ARG_COUNT(3, {
        SET_ERROR("setz requires 3 argument", USE_HELP("setz"));
    });

    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, "map name must be a string");
        TYPE_OF(2, STR, "key must be a string");
        TYPE_OF(3, ANY, "");
    });

    EXE_CMD(setz);
}

// end setz

// delz
DEFINE_CMD(delz) {
    INIT_ARGS(ARG(mapname); ARG(keyname));

    if (map_exists(MCTX->_user_maps, mapname->data._str) == 0) {
        SET_ERROR("map ", C_STR(mapname), " does not exist");
        return;
    }

    Map *user_map = map_get(MCTX->_user_maps, mapname->data._str);

    if (map_exists(user_map, keyname->data._str) == 0) {
        SET_ERROR("key '", C_STR(keyname), "' does not exits in map '", C_STR(mapname), "'");
        return;
    }

    map_delete(user_map, keyname->data._str);
    SET_RESULT_MSG("Done");
}

CMD_WRAP(delz) {
    CREATE_HELP(
        USAGE("delz map key"),
        DESC("delete a value from a user defined map"),
        ARG_DESC("map", "the name of the map", "string"),
        ARG_DESC("key", "the name of the key", "string"));

    ENFORCE_ARG_COUNT(2, {
        SET_ERROR("delz requires 2 arguments.", USE_HELP("delz"));
    });

    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, "map name must be a string");
        TYPE_OF(2, STR, "key must a string");
    });

    EXE_CMD(delz);
}
// end delz
