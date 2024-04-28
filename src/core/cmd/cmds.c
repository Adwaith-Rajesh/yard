#include "core/cmd/cmds.h"

#include <stdio.h>

#include "core/cmd/parser.h"
#include "mctx.h"

static Container *get_next_arg(ListNode *node) {
    node = node->next;
    Container *data = node->data;
    return data;
}

// static void CMD(YardMasterCtx *mctx, ParserCtx *pctx, )

DEFINE_CMD(get) {
    INIT_ARGS(ARG(keyname));
    Container *val = map_get(MCTX->_default_map, keyname->data._str);
}

DEFINE_CMD(set) {
    INIT_ARGS(ARG(keyname); ARG(val));

    map_set(mctx->_default_map, keyname, container_clone(val));

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
