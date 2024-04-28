#ifndef YARD_CORE_CMD_CMDS_H
#define YARD_CORE_CMD_CMDS_H

#include "core/cmd/parser.h"
#include "core/ds/dc.h"
#include "core/ds/list.h"
#include "mctx.h"

// creates the function signature for the command
#define DEFINE_CMD(CMD) static void CMD(YardMasterCtx *mctx, ParserCtx *pctx, CmdResult *res)

#define CMD_WRAP(CMD) void CMD##_wrap(YardMasterCtx *mctx, ParserCtx *pctx, CmdResult *res)

#define MCTX mctx

#define PCTX pctx

#define RES res

#define CHECK_HELP(H_MSG)                       \
    do {                                        \
        if (mctx == NULL && pctx == NULL) {     \
            res->result_type = R_HELP;          \
            str_clear(res->emsg);               \
            str_append_charp(res->emsg, H_MSG); \
            return;                             \
        }                                       \
    } while (0)

#define SET_ERROR(ERR_MSG)                    \
    do {                                      \
        res->result_type = R_ERROR;           \
        str_clear(res->emsg);                 \
        str_append_charp(res->emsg, ERR_MSG); \
    } while (0)

#define ENFORCE_ARG_COUNT(COUNT, B)           \
    int arg_count = COUNT + 1;                \
    do {                                      \
        if (pctx->cmd->size != (COUNT + 1)) { \
            B;                                \
        }                                     \
    } while (0)

#define ENFORCE_ARG_TYPE(B)               \
    do {                                  \
        ListNode *node = pctx->cmd->head; \
        int a_count = 0;                  \
        while (a_count < arg_count) {     \
            B;                            \
            a_count++;                    \
            node = node->next;            \
        }                                 \
    } while (0)

#define TYPE_OF(ARG_IDX, TYPE, ERROR)                                                    \
    if (node != NULL && a_count == ARG_IDX && ((Container *)node->data)->type != TYPE) { \
        ERROR;                                                                           \
    }

#define INIT_ARGS(B)                  \
    ListNode *head = pctx->cmd->head; \
    B;

#define ARG(ARG_NAME)  \
    head = head->next; \
    Container *ARG_NAME = head->data

#define EXE_CMD(CMD) CMD(mctx, pctx, res)

#define SET_RESULT_MSG(MSG)               \
    do {                                  \
        res->result_type = R_MSG;         \
        str_append_charp(res->emsg, MSG); \
    } while (0)

// get key
CMD_WRAP(get);

// set key val
CMD_WRAP(set);

// del key
CMD_WRAP(del);

#endif

// context can store the help message
