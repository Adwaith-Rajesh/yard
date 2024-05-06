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
        SET_ERROR("key does not exist");
        return;
    }

    gen_result(val, RES);
}

CMD_WRAP(get) {
    CHECK_HELP("Usage:\n\tget keyname");
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("get requires 1 argument, key");
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
    CHECK_HELP("Usage:\n\tset keyname value");
    ENFORCE_ARG_COUNT(2, {
        SET_ERROR("set requires 2 arguments, key and val");
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
        SET_ERROR("key does not exist");
        return;
    }

    map_delete(MCTX->_default_map, keyname->data._str);
    SET_RESULT_MSG("Done");
}

CMD_WRAP(del) {
    CHECK_HELP("Usage:\n\tdel keyname");
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("del requires 1 argument, key");
        return;
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

#define GET_HELP_CALL(CMD) (((Container *)map_get(MCTX->_commands, CMD))->data._fn)(NULL, NULL, res)

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
        SET_ERROR("command does not exist");
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
    CHECK_HELP("Usage:\n\tpushl val\nPush a value to the left of the list.");
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("pushl requires one argument");
        return;
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
    CHECK_HELP("Usage:\n\tpopl\nPop a value from the left of the list");
    ENFORCE_ARG_COUNT(0, {
        SET_ERROR("popl does not accept any args");
        return;
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
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("pushr requires one argument");
        return;
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
    CHECK_HELP("Usage:\n\tpopl\nPop a value from the right of the list");
    ENFORCE_ARG_COUNT(0, {
        SET_ERROR("popr does not accept any args");
        return;
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
    CHECK_HELP("Usage:\n\tcreate type name\n\tcreates a new list/map other than the default one\n\ttype: list|map");
    ENFORCE_ARG_COUNT(2, {
        SET_ERROR("create requires 2 argument. use 'help create' for more info");
        return;
    });

    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, "create type name must be a string");
        TYPE_OF(2, STR, "name of the type must be a string");
    });

    EXE_CMD(create);
}

// end create
