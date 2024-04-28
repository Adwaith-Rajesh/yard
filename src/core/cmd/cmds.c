#include "core/cmd/cmds.h"

#include <stdio.h>

#include "core/cmd/parser.h"
#include "mctx.h"

static void gen_result(Container *val, CmdResult *res) {
    switch (val->type) {
        case STR:
            res->result_type = R_STR;
            str_append_charp(res->_str, val->data._str);
            break;
        case INT:
            res->result_type = R_INT;
            res->_int = val->data._int;
            break;
        case FLOAT:
            res->result_type = R_FLOAT;
            res->_float = val->data._float;
            break;
        default:
            return;
    }
}

// static void CMD(YardMasterCtx *mctx, ParserCtx *pctx, )

// get keyname

DEFINE_CMD(get) {
    INIT_ARGS(ARG(keyname));
    Container *val = map_get(MCTX->_default_map, keyname->data._str);

    if (val == NULL) {
        SET_ERROR("key does not exists");
        return;
    }

    gen_result(val, RES);
}

CMD_WRAP(get) {
    CHECK_HELP("Usage: get keyname");
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("del required 1 argument, key");
        return;
    });
    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, {
            SET_ERROR("keyname must be a string");
            return;
        });
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
    CHECK_HELP("Usage: set keyname value");
    ENFORCE_ARG_COUNT(2, {
        SET_ERROR("set requires 2 arguments, key and val");
    });

    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, {
            SET_ERROR("key must be a string");
            return;
        });
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
    CHECK_HELP("Usage: del keyname");
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("del requires 1 argument, key");
    });
    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, {
            SET_ERROR("key must be a string");
            return;
        });
    });

    EXE_CMD(del);
}
