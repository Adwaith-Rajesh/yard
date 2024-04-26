#include "core/cmd/cmds.h"

#include <stdio.h>

#include "core/cmd/parser.h"
#include "yard.h"

CMD_SIG(get) {
    printf("this is the get command\n");
}

CMD_SIG(set) {
    printf("this is the set command\n");
}
