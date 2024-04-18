#ifndef YARD_UTILS_LOG_H
#define YARD_UTILS_LOG_H

typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR
} Level;

#define level_str(level) #level

#define LOG_ERROR(msg) log_msg(ERROR, msg, __FILE__, __LINE__)
#define LOG_WARN(msg) log_msg(WARN, msg, __FILE__, __LINE__)
#define LOG_INFO(msg) log_msg(INFO, msg, __FILE__, __LINE__)
#define LOG_DEBUG(msg) log_msg(DEBUG, msg, __FILE__, __LINE__)

void log_msg(Level level, const char *msg, const char *filename, int lineno);

#ifdef LOG_IMPLEMENTATION

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

#endif

#endif
