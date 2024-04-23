#ifndef __ISA_H__
#define __ISA_H__

#include <common.h>

#define CSR_ARR_LEN 4096
#define CSR_MSTATUS 0x300
#define CSR_MTVEC   0x305
#define CSR_MEPC    0x341
#define CSR_MCAUSE  0x342

typedef struct {
    word_t gpr[32];
    word_t csr[CSR_ARR_LEN];
    word_t pc;
} CPUStruct;

extern CPUStruct cpu;

void initISA();

#endif
