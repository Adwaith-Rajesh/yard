#include "core/cmd/cmds.h"

#include <stdio.h>

#include "core/cmd/parser.h"
#include "mctx.h"

static Container *get_next_arg(ListNode *node) {
    node = node->next;
    Container *data = node->data;
    return data;
}

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

DEFINE_CMD(get) {
    INIT_ARGS(ARG(keyname));
    Container *val = map_get(MCTX->_default_map, keyname->data._str);

    if (val == NULL) {
        SET_ERROR("key does not exists");
        return;
    }

    gen_result(val, RES);
}

DEFINE_CMD(set) {
    INIT_ARGS(ARG(keyname); ARG(val));
    map_set(MCTX->_default_map, keyname->data._str, container_clone(val));
    SET_RESULT_MSG("Done\n");
}

// wrappers the contains extra info about the commands

CMD_WRAP(get) {
    CHECK_HELP("Usage: get keyname\n");
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("Please provide key name\n");
        return;
    });
    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, {
            SET_ERROR("keyname must be a string\n");
            return;
        });
    });

    EXE_CMD(get);
}

CMD_WRAP(set) {
    CHECK_HELP("Usage: set keyname value\n");
    ENFORCE_ARG_COUNT(2, {
        SET_ERROR("set requires 2 arguments\n");
    });

    ENFORCE_ARG_TYPE({
        TYPE_OF(1, STR, {
            SET_ERROR("keyname must be a string\n");
            return;
        });
    });

    EXE_CMD(set);
}
