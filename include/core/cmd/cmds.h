#ifndef YARD_CORE_CMD_CMDS_H
#define YARD_CORE_CMD_CMDS_H

#include "core/cmd/cmd_utils.h"

// in yard c
void _container_print_list(ListNode *node);

void _container_print_map(MapEntry *map);

void _container_free(void *data);

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

// delete
CMD_WRAP(delete);

// getz
CMD_WRAP(getz);

// setz
CMD_WRAP(setz);

// delz
CMD_WRAP(delz);

#endif
