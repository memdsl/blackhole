#include <isa/difftest.h>
#include <isa/isa.h>
#include <isa/gpr.h>

bool checkISADifftestReg(CPUStruct *cpu_ref, vaddr_t pc) {
    for (int i = 0; i < 32; i++) {
        if (cpu_ref->gpr[i] != cpu.gpr[i]) {
            printf("[difftest] error at pc: " FMT_WORD "\n", pc);
            return false;
        }
    }
    return true;
}

void printfISADifftestReg(CPUStruct *cpu_ref, vaddr_t pc) {
    char *space_num = (char *)"";
    char *space_gpr = (char *)"";
    char *error_str = (char *)"";
    for (int i = 0; i < 32; i++) {
        const char *name = getISAGPRName(i);
        space_num = (i < 10) ? (char *)" " : (char *)"";
        space_gpr = (strcmp(name, "s10") != 0 &&
                     strcmp(name, "s11") != 0) ? (char *)" " : (char *)"";
        error_str = (cpu_ref->gpr[i] != cpu.gpr[i]) ?
                        (char *)ANSI_FMT("*", ANSI_FG_RED) : (char *)"";
        printf("[difftest] gpr i: %d%s dut data: %s%s = " FMT_WORD \
               " ref data: %s%s = " FMT_WORD "%s\n",
               i,
               space_num,
               space_gpr,
               name,
               cpu.gpr[i],
               space_gpr,
               name,
               cpu_ref->gpr[i],
               error_str);
    }
}
