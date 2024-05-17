#include "core/cmd/cmd_utils.h"

#include <stdarg.h>
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
    (void)mctx;
    if (pctx == NULL) {  // pctx is set to null inorder to get the help message
        res->result_type = R_HELP;
        str_clear(res->emsg);
        str_append_charp(res->emsg, help_msg);
        return 1;
    }
    return 0;
}

int create_help(ParserCtx *pctx, YardMasterCtx *mctx, CmdResult *res, const char *usage, const char *desc, ...) {
    Str *help_string = str_create(mctx->allocator, mctx->deallocator, mctx->reallocator);

    str_append_charp(help_string, usage);
    str_append_charp(help_string, desc);
    str_append_charp(help_string, "\tArguments:\n");

    va_list args;
    va_start(args, desc);

    const char *arg = va_arg(args, const char *);
    if (arg == NULL) {  // we dont have any args
        str_append_charp(help_string, "\t\tcommand does not accept any arguments");
    }

    while (arg != NULL) {
        str_append_charp(help_string, arg);
        arg = va_arg(args, const char *);
    }

    va_end(args);
    int r = check_help(pctx, mctx, res, help_string->str);
    str_free(help_string);
    return r;
}

void set_error(CmdResult *res, ...) {
    va_list args;
    va_start(args, res);

    const char *arg = va_arg(args, const char *);
    str_clear(res->emsg);
    while (arg != NULL) {
        res->result_type = R_ERROR;
        str_append_charp(res->emsg, arg);
        arg = va_arg(args, const char *);
    }
    va_end(args);
}

int check_arg_type(ArgType types[], int arg_count, ParserCtx *pctx, CmdResult *res) {
    ListNode *head = pctx->cmd->head;
    ListNode *temp = head->next;
    for (int i = 1; i < arg_count; i++) {
        if (types[i].type == ANY) {
            continue;
        }

        if ((int)(((Container *)(temp->data))->type) != (int)(types[i].type)) {
            set_error(res, types[i].err_msg, NULL);
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
