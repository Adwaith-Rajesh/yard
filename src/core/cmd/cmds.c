#include "core/cmd/cmds.h"

#include <stdio.h>

#include "core/cmd/parser.h"
#include "mctx.h"

// satatic int CMD(YardMasterCtx *mctx, ParserCtx *pctx, )

DEFINE_CMD(get) {
    res->ok = 1;
    res->result_type = R_INT;
    res->_int = 2;
}

DEFINE_CMD(set) {
    (void)mctx;
    (void)pctx;
    printf("this is the set command\n");
}

DEFINE_CMD(del) {
    (void)mctx;
    (void)pctx;
    printf("this is the del commands\n");
}

// wrappers the contains extra info about the commands

CMD_WRAP(get) {
    CHECK_HELP("Usage: get keyname\n");
    ENFORCE_ARG_COUNT(1, {
        SET_ERROR("Please provide key name\n");
        return;
    });

    EXE_CMD(get);
}

CMD_WRAP(set) {
    CHECK_HELP("Usage: set keyname value\n");
}
