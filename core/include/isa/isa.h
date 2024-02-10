#ifndef __ISA_H__
#define __ISA_H__

#include <common.h>

typedef struct {
    word_t gpr[32];
    word_t pc;
} CPUStruct;

extern CPUStruct cpu;

void initISA();

#endif
