#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/cmd/parser.h"
#include "core/ds/str.h"
#include "yard.h"

#define SET_ERROR_MSG(MSG)      \
    res->ok = 0;                \
    res->result_type = R_ERROR; \
    str_append_charp(res->emsg, MSG)

Str *get_cmd(Str *str) {
    int c;
    while ((c = getc(stdin))) {
        if (c == EOF || c == '\n') {
            break;
        }
        str_append_char(str, c);
    }
    return str;
}

int check_quit(Str *str) {
    return strncmp(str->str, "q", 1) == 0;
}

inline Container *_get_first_node(List *cmd) {
    return (Container *)cmd->head->data;
}

inline void _exec_cmd(YardMasterCtx *mctx, ParserCtx *pctx, CmdResult *res, char *cmd_name) {
    (((Container *)map_get(mctx->_commands, cmd_name))->data._fn)(mctx, pctx, res);
}

void call_cmd(YardMasterCtx *mctx, ParserCtx *pctx, CmdResult *res) {
    if (pctx->cmd->size == 0) {
        return;
    }

    Container *first_node = _get_first_node(pctx->cmd);
    if (first_node->type != STR) {
        SET_ERROR_MSG("Not a valid command\n");
        return;
    }

    if (map_exists(mctx->_commands, first_node->data._str) == 0) {
        SET_ERROR_MSG("Unknown command\n");
        return;
    }

    _exec_cmd(mctx, pctx, res, first_node->data._str);
}

int main() {
    printf("%s", LICENSE);

    YardMasterCtx *mctx = mctx_create(malloc, free, realloc);
    ParserCtx *pctx = parser_init(mctx->allocator, mctx->deallocator);
    Str *str = STR_FROM_MCTX(mctx);
    CmdResult *res = result_create(mctx);

    while (1) {
        printf("yard >> ");
        get_cmd(str);

        if (check_quit(str)) {
            break;
        }

        parse_cmd(pctx, str->str);
        call_cmd(mctx, pctx, res);

        if (res->ok != 1 && res->result_type == R_ERROR) {
            printf("ERROR: ");
            str_print(res->emsg);
            res->ok = 1;
            str_clear(res->emsg);
        }

        // list_print(pctx->cmd);

        printf("\n");
        str_clear(str);
    }

    str_free(str);
    result_free(res, mctx);
    mctx_free(mctx);
    parser_free(pctx);
}
