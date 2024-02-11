#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <common.h>
#include <stdio.h>
#include <util/log.h>

#define LOG_PURE(format, ...) \
    _LOG(format "\n", \
         ## __VA_ARGS__)

#define LOG_PURE_COLOR(format, ...) \
    _LOG(ANSI_FMT(format, ANSI_FG_BLUE) "\n", \
         ## __VA_ARGS__)

#define LOG_FILE(format, ...) \
    _LOG(ANSI_FMT("[%s:%d %s] " format, ANSI_FG_BLUE) "\n", \
         __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#define ASSERT(cond, format, ...) \
    do { \
        if (!(cond)) { \
            MUXDEF(CONFIG_TARGET_AM, \
                   printf(ANSI_FMT(format, ANSI_FG_RED) "\n", ## __VA_ARGS__), \
                  (fflush(stdout), fprintf(stderr, ANSI_FMT(format, ANSI_FG_RED) "\n", ##  __VA_ARGS__))); \
            IFNDEF(CONFIG_TARGET_AM, extern FILE* log_fp; fflush(log_fp)); \
            extern void printfCPUAssertFailMsg(); \
            printfCPUAssertFailMsg(); \
            assert(cond); \
        } \
    } while (0)

#define PANIC(format, ...) ASSERT(0, format, ## __VA_ARGS__)

#endif
