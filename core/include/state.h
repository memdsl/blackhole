#ifndef __STATE_H__
#define __STATE_H__

#include <common.h>

enum { CPU_RUNNING, CPU_STOP, CPU_END, CPU_ABORT, CPU_QUIT };

typedef struct {
    int state;
    vaddr_t halt_pc;
    uint32_t halt_ret;
} CPUState;

extern CPUState cpu_state;

int judgeCPUStateIsBad();
void setCPUState(int state, vaddr_t pc, int ret);

#endif
