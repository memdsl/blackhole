#include <cpu/sim.h>
#include <isa/isa.h>
#include <isa/csr.h>

const char *csr_name_arr[] = {
    "mstatus", "mtvec  ", "mepc   ",  "mcause "
};
int csr_index_arr[] = {
    CSR_MSTATUS,
    CSR_MTVEC,
    CSR_MEPC,
    CSR_MCAUSE
};

static int checkISACSRIsValid(int id) {
    IFDEF(CONFIG_RT_CHECK, assert(id >= 0 && id < 4));
    return id;
}

word_t getISACSR(int id) {
    id = checkISACSRIsValid(id);
#if CFLAGS_CPU_TYPE_ML1
    switch (id) {
        case 0:  return top->io_pTrace_pCSRRd_bRdMSTAData;
        case 1:  return top->io_pTrace_pCSRRd_bRdMTVEData;
        case 2:  return top->io_pTrace_pCSRRd_bRdMEPCData;
        case 3:  return top->io_pTrace_pCSRRd_bRdMCAUData;
        default: return 0;
    }
#elif CFLAGS_CPU_TYPE_ML2
     return 0;
#endif
}

word_t getISACSRData(const char *csr, bool *success) {
    word_t data = 0;

    for (int i = 0; i < ARRLEN(csr_name_arr); i++) {
        if (strcmp(csr_name_arr[i], csr) == 0) {
            data = getISACSR(i);
            *success = true;
            break;
        }
    }

    return data;
}

const char *getISACSRName(int id) {
    return csr_name_arr[checkISACSRIsValid(id)];
}

void printfISACSRData() {
    char *exist_str = (char *)"";
    for (int i = 0; i < ARRLEN(csr_name_arr); i++) {
        exist_str = (getISACSR(i) != 0) ? (char *)ANSI_FMT("*", ANSI_FG_GREEN) :
                                          (char *)"";
        LOG_PURE("[sdb] [csr] i: %d  data: %s = " FMT_WORD "%s",
                 i,
                 csr_name_arr[i],
                 getISACSR(i),
                 exist_str);
    }
    LOG_PURE();
}
