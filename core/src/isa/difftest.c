#include <isa/difftest.h>
#include <isa/isa.h>
#include <isa/gpr.h>
#include <isa/csr.h>

extern int csr_index_arr[];

bool checkISADifftestReg(CPUStruct *cpu_ref, vaddr_t pc) {
    bool error_flag = false;

    for (int i = 0; i < 32; i++) {
        if (cpu.gpr[i] != cpu_ref->gpr[i]) {
            error_flag = true;
            break;
        }
    }
    for (int i = 0; i < 4; i++) {
        int csr_index = csr_index_arr[i];
        if (cpu.csr[csr_index] != cpu_ref->csr[csr_index]) {
            error_flag = true;
            break;
        }
    }

    if (error_flag) {
        printf("[difftest] error at pc: " FMT_WORD "\n", pc);
        return false;
    }
    else {
        return true;
    }
}

void printfISADifftestReg(CPUStruct *cpu_ref, vaddr_t pc) {
    char *space_num = (char *)"";
    char *space_gpr = (char *)"";
    char *error_str = (char *)"";

    for (int i = 0; i < 32; i++) {
        const char *gpr_name = getISAGPRName(i);
        space_num = (i < 10) ? (char *)" " : (char *)"";
        space_gpr = (strcmp(gpr_name, "s10") != 0 &&
                     strcmp(gpr_name, "s11") != 0) ? (char *)" " : (char *)"";
        error_str = (cpu_ref->gpr[i] != cpu.gpr[i]) ?
                        (char *)ANSI_FMT("*", ANSI_FG_RED) : (char *)"";
        LOG_PURE("[difftest] gpr i: %d%s dut data: %s%s = " FMT_WORD \
                 " ref data: %s%s = " FMT_WORD "%s",
                 i,
                 space_num,
                 space_gpr,
                 gpr_name,
                 cpu.gpr[i],
                 space_gpr,
                 gpr_name,
                 cpu_ref->gpr[i],
                 error_str);
    }

    for (int i = 0; i < 4; i++) {
        int csr_index = csr_index_arr[i];
        const char *csr_name = getISACSRName(i);
        error_str = (cpu.csr[csr_index] != cpu_ref->csr[csr_index]) ?
                        (char *)ANSI_FMT("*", ANSI_FG_RED) : (char *)"";
        LOG_PURE("[difftest] csr %s dut data: " FMT_WORD " ref data: " FMT_WORD "%s",
                 csr_name,
                 cpu.csr[csr_index],
                 cpu_ref->csr[csr_index],
                 error_str);
    }
}
