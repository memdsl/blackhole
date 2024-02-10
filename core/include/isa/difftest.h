#ifndef __ISA_DIFFTEST_H__
#define __ISA_DIFFTEST_H__

#include <common.h>
#include <isa/isa.h>

bool checkISADifftestGPR(CPUStruct *cpu_ref, vaddr_t pc);
void printfISADifftest(CPUStruct *cpu_ref, vaddr_t pc);

#endif
