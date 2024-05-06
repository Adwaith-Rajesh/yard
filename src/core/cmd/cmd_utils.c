#include "core/cmd/cmd_utils.h"

#include <stdio.h>

void gen_result(Container *val, CmdResult *res) {
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

int check_help(ParserCtx *pctx, YardMasterCtx *mctx, CmdResult *res, const char *help_msg) {
    if (mctx == NULL && pctx == NULL) {
        res->result_type = R_HELP;
        str_clear(res->emsg);
        str_append_charp(res->emsg, help_msg);
        return 1;
    }
    return 0;
}

void set_error(CmdResult *res, const char *err_msg) {
    res->result_type = R_ERROR;
    str_clear(res->emsg);
    str_append_charp(res->emsg, err_msg);
}

int check_arg_type(ArgType types[], int arg_count, ParserCtx *pctx, CmdResult *res) {
    ListNode *head = pctx->cmd->head;
    ListNode *temp = head->next;
    for (int i = 1; i < arg_count; i++) {
        if (types[i].type == ANY) {
            continue;
        }

        if ((int)(((Container *)(temp->data))->type) != (int)(types[i].type)) {
            set_error(res, types[i].err_msg);
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

void set_result_msg(CmdResult *res, const char *msg) {
    res->result_type = R_MSG;
    str_append_charp(res->emsg, msg);
}
