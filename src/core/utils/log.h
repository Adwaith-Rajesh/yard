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

#endif
