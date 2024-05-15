#ifndef YARD_CORE_CMD_CMD_UTILS_H
#define YARD_CORE_CMD_CMD_UTILS_H

#include <string.h>

#include "core/cmd/parser.h"
#include "core/ds/dc.h"
#include "core/ds/list.h"
#include "mctx.h"

#define ERR_MSG_SIZE 2048  // It think its big enough

typedef enum {
    ANY = CMD_FN_PTR + 1
} arg_t;

typedef struct {
    int type;
    char err_msg[ERR_MSG_SIZE];
} ArgType;

// creates the function signature for the command
#define DEFINE_CMD(CMD) static void CMD(YardMasterCtx *mctx, ParserCtx *pctx, CmdResult *res)

#define CMD_WRAP(CMD) void CMD##_wrap(YardMasterCtx *mctx, ParserCtx *pctx, CmdResult *res)

#define MCTX mctx

#define PCTX pctx

#define RES res

#define GET_ARG_COUNT() pctx->cmd->size - 1

#define CHECK_HELP(H_MSG)                              \
    do {                                               \
        if (check_help(PCTX, MCTX, RES, H_MSG) == 1) { \
            return;                                    \
        }                                              \
    } while (0)

#define SET_ERROR(...) set_error(RES, __VA_ARGS__, NULL)

#define ENFORCE_ARG_COUNT(COUNT, B)           \
    int arg_count = COUNT + 1;                \
    do {                                      \
        if (pctx->cmd->size != (COUNT + 1)) { \
            B;                                \
            return;                           \
        }                                     \
    } while (0)

#define ENFORCE_ARG_TYPE(B)                                     \
    ArgType arg_types[arg_count];                               \
    B;                                                          \
    if (check_arg_type(arg_types, arg_count, PCTX, RES) == 0) { \
        return;                                                 \
    }

#define TYPE_OF(ARG_IDX, TYPE, ERR_MSG)  \
    arg_types[ARG_IDX].type = (int)TYPE; \
    strncpy(arg_types[ARG_IDX].err_msg, ERR_MSG, ERR_MSG_SIZE)

#define INIT_ARGS(B)                  \
    ListNode *head = pctx->cmd->head; \
    B;

#define ARG(ARG_NAME)  \
    head = head->next; \
    Container *ARG_NAME = head->data

#define EXE_CMD(CMD) CMD(mctx, pctx, res)

#define SET_RESULT_MSG(MSG) set_result_msg(RES, MSG)

#define UN_USED(ARG) (void)ARG

#define NO_ARG_TYPE UN_USED(arg_count)

void gen_result(Container *val, CmdResult *res);

int check_help(ParserCtx *pctx, YardMasterCtx *mctx, CmdResult *res, const char *help_msg);

void set_error(CmdResult *res, ...);

int check_arg_type(ArgType types[], int arg_count, ParserCtx *pctx, CmdResult *res);

void set_result_msg(CmdResult *res, const char *msg);

#endif
