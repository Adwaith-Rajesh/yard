#include "core/utils/log.h"

#include <stdio.h>
#include <stdlib.h>

void log_msg(Level level, const char *msg, const char *filename, int lineno) {
    char print_str[2048];
    snprintf(print_str, 2048, "[%s](%s:%d): %s\n",
             (level == DEBUG) ? level_str(DEBUG) : ((level == INFO) ? level_str(INFO) : ((level == WARN) ? level_str(WARN) : level_str(ERROR))),
             filename,
             lineno,
             msg);
    fprintf(stderr, "%s", print_str);
}
