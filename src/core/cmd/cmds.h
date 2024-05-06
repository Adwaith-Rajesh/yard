#ifndef YARD_CORE_CMD_CMDS_H
#define YARD_CORE_CMD_CMDS_H

#include "core/cmd/cmd_utils.h"

// get key
CMD_WRAP(get);

// set key val
CMD_WRAP(set);

// del key
CMD_WRAP(del);

// help
CMD_WRAP(help);

// pushl val
CMD_WRAP(pushl);

// popl
CMD_WRAP(popl);

// pushr
CMD_WRAP(pushr);

// popr
CMD_WRAP(popr);

// create
CMD_WRAP(create);

#endif
