#ifndef __TRACE_H__
#define __TRACE_H__

#include <common.h>

void recordDebugITrace(char *logbuf);
void printfDebugITrace(char *type);

void printfDebugMTrace(char *type,
                       char *dir,
                       word_t addr,
                       word_t data,
                       word_t len);

void printfDebugFTrace(char *type,
                       bool inst_func_call,
                       bool inst_func_ret,
                       word_t pc,
                       word_t dpnc);
void freeDebugFTrace();

void printfDebugDTrace(char *type,
                       char *name,
                       char *dir,
                       word_t addr,
                       word_t data);

void printfDebugETrace(char *type, int dir);

void initDebugTrace(char *elf_file);

#endif
