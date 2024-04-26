#ifndef YARD_CORE_CMD_CMDS_H
#define YARD_CORE_CMD_CMDS_H

#include "core/cmd/parser.h"
#include "yard.h"

// creates the function signature for the command
#define CMD_SIG(CMD) int CMD(YardMasterCtx *mctx, ParserCtx *pctx)

CMD_SIG(get);
CMD_SIG(set);

#endif
