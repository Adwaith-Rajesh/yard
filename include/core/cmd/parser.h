// parse the command received from the user
// the parser here also kind of works like a lexer. :)

#ifndef YARD_CMD_PARSER_H
#define YARD_CMD_PARSER_H

#include <stddef.h>

#include "core/ds/list.h"

typedef struct {
    void *(*allocator)(size_t);
    void (*deallocator)(void *);
    List *cmd;  // stores the last parsed command
} ParserCtx;

ParserCtx *parser_init(void *(*allocator)(size_t), void (*deallocator)(void *));

void parser_free(ParserCtx *pctx);

void parse_cmd(ParserCtx *pctx, const char *cmd);

#endif
