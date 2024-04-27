#ifndef YARD_CORE_CMD_CMDS_H
#define YARD_CORE_CMD_CMDS_H

#include "core/cmd/parser.h"
#include "mctx.h"

// creates the function signature for the command
#define DEFINE_CMD(CMD) static void CMD(YardMasterCtx *mctx, ParserCtx *pctx, CmdResult *res)

#define CMD_WRAP(CMD) void CMD##_wrap(YardMasterCtx *mctx, ParserCtx *pctx, CmdResult *res)

#define CHECK_HELP(H_MSG)                       \
    do {                                        \
        if (mctx == NULL && pctx == NULL) {     \
            res->ok = 1;                        \
            res->result_type = R_HELP;          \
            str_clear(res->emsg);               \
            str_append_charp(res->emsg, H_MSG); \
        }                                       \
    } while (0)

#define SET_ERROR(ERR_MSG)                    \
    do {                                      \
        res->ok = 0;                          \
        res->result_type = R_ERROR;           \
        str_clear(res->emsg);                 \
        str_append_charp(res->emsg, ERR_MSG); \
    } while (0)

#define ENFORCE_ARG_COUNT(COUNT, B)           \
    do {                                      \
        if (pctx->cmd->size != (COUNT + 1)) { \
            B;                                \
        }                                     \
    } while (0)

#define EXE_CMD(CMD) CMD(mctx, pctx, res)

// get key
CMD_WRAP(get);

// set key val
CMD_WRAP(set);

// del key
CMD_WRAP(del);

#endif

// context can store the help message
