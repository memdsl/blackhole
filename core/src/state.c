#include <state.h>

CPUState cpu_state = { .state = CPU_STOP };

int judgeCPUStateIsBad() {
    int good = (cpu_state.state == CPU_END && cpu_state.halt_ret == 0) ||
               (cpu_state.state == CPU_QUIT);
    return !good;
}

void setCPUState(int state, vaddr_t pc, int ret) {
    cpu_state.state    = state;
    cpu_state.halt_pc  = pc;
    cpu_state.halt_ret = ret;
}
