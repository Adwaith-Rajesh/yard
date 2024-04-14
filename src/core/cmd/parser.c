#include "core/cmd/parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/ds/dc.h"
#include "core/ds/list.h"

const char *keywords[] = {
    "get",
    "set",
    "del",
};

static void _list_container_free(void *data) {
    if (data == NULL) {
        return;
    }
    container_free((Container *)data);
}

// for debugging purposes
static void _list_container_print(ListNode *node) {
    if (node == NULL) {
        return;
    }

    container_print((Container *)node->data);
}

// return 1 if the given digit is an integer
static int is_int(const char *str) {
    int c;
    while ((c = *str++)) {
        if (!isdigit((const char)c)) {
            return 0;
        }
    }
    return 1;
}

// return 1 if str is float else 0
// code stolen from: https://stackoverflow.com/a/45555413/14617085
static int is_float(const char *str) {
    int len;
    float dummy = 0.0;
    if (sscanf(str, "%f %n", &dummy, &len) == 1 && len == (int)strlen(str))
        return 1;
    return 0;
}

// create container based on the type of the data
static Container *_create_container(const char *str, void *(*allocator)(size_t), void (*deallocator)(void *)) {
    if (is_int(str)) {
        return create_int_container(atoi(str), allocator, deallocator);
    }

    if (is_float(str)) {
        return create_float_container(atof(str), allocator, deallocator);
    }

    return create_str_container(str, allocator, deallocator);
}

// The "parsing" stuff (we are technically lexing)

ParserCtx *parser_init(void *(*allocator)(size_t), void (*deallocator)(void *)) {
    ParserCtx *new_pctx = allocator(sizeof(ParserCtx));
    new_pctx->allocator = allocator;
    new_pctx->deallocator = deallocator;

    // initialize the cmd list
    List *cmd_list = list_init(allocator, deallocator, _list_container_free, _list_container_print);
    new_pctx->cmd = cmd_list;
    return new_pctx;
}

void parser_free(ParserCtx *pctx) {
    list_free(pctx->cmd);
    pctx->deallocator(pctx);
}

void parse_cmd(ParserCtx *pctx, const char *cmd) {
    list_truncate(pctx->cmd);

    size_t cmd_len = strlen(cmd);
    char *cmd_cpy = pctx->allocator(cmd_len + 1);
    strncpy(cmd_cpy, cmd, cmd_len);

    // will be set to 1 if a " is encountered and 0 if another " is encountered
    int in_str_quote = 0;
    char *curr_str_v = NULL;  // current string view (part of the command string)

    int c = 0;
    size_t i = 0;
    while ((c = cmd_cpy[i]) != '\0') {
        switch (c) {
            case ' ': {
                if (in_str_quote != 0) {
                    break;
                }
                cmd_cpy[i] = '\0';

                if (curr_str_v != NULL) {
                    list_pushr(pctx->cmd, _create_container(curr_str_v, pctx->allocator, pctx->deallocator));
                }
                curr_str_v = NULL;
                break;
            }

            case '"':
            case '\'': {
                if (in_str_quote == 0 && curr_str_v == NULL) {
                    curr_str_v = &cmd_cpy[i];
                }

                in_str_quote = !in_str_quote;
                break;
            }

            default: {
                if (curr_str_v == NULL) {
                    curr_str_v = &cmd_cpy[i];
                }
            }
        }
        i++;

        if (cmd_cpy[i] == '\0' && curr_str_v != NULL) {
            list_pushr(pctx->cmd, _create_container(curr_str_v, pctx->allocator, pctx->deallocator));
            curr_str_v = NULL;
        }
    }
    pctx->deallocator(cmd_cpy);
}
