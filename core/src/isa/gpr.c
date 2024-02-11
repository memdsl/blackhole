#include <cpu/sim.h>
#include <isa/gpr.h>

const char *gpr_name_arr[] = {
    "$0", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
    "s0", "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
    "a6", "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

static int checkISAGPRIsValid(int id) {
    IFDEF(CONFIG_RT_CHECK, assert(id >= 0 && id < 32));
    return id;
}

word_t getISAGPR(int id) {
    id = checkISAGPRIsValid(id);
    switch (id) {
        case 0:  return top->io_pTrace_pGPRRd_bRdData0;
        case 1:  return top->io_pTrace_pGPRRd_bRdData1;
        case 2:  return top->io_pTrace_pGPRRd_bRdData2;
        case 3:  return top->io_pTrace_pGPRRd_bRdData3;
        case 4:  return top->io_pTrace_pGPRRd_bRdData4;
        case 5:  return top->io_pTrace_pGPRRd_bRdData5;
        case 6:  return top->io_pTrace_pGPRRd_bRdData6;
        case 7:  return top->io_pTrace_pGPRRd_bRdData7;
        case 8:  return top->io_pTrace_pGPRRd_bRdData8;
        case 9:  return top->io_pTrace_pGPRRd_bRdData9;
        case 10: return top->io_pTrace_pGPRRd_bRdData10;
        case 11: return top->io_pTrace_pGPRRd_bRdData11;
        case 12: return top->io_pTrace_pGPRRd_bRdData12;
        case 13: return top->io_pTrace_pGPRRd_bRdData13;
        case 14: return top->io_pTrace_pGPRRd_bRdData14;
        case 15: return top->io_pTrace_pGPRRd_bRdData15;
        case 16: return top->io_pTrace_pGPRRd_bRdData16;
        case 17: return top->io_pTrace_pGPRRd_bRdData17;
        case 18: return top->io_pTrace_pGPRRd_bRdData18;
        case 19: return top->io_pTrace_pGPRRd_bRdData19;
        case 20: return top->io_pTrace_pGPRRd_bRdData20;
        case 21: return top->io_pTrace_pGPRRd_bRdData21;
        case 22: return top->io_pTrace_pGPRRd_bRdData22;
        case 23: return top->io_pTrace_pGPRRd_bRdData23;
        case 24: return top->io_pTrace_pGPRRd_bRdData24;
        case 25: return top->io_pTrace_pGPRRd_bRdData25;
        case 26: return top->io_pTrace_pGPRRd_bRdData26;
        case 27: return top->io_pTrace_pGPRRd_bRdData27;
        case 28: return top->io_pTrace_pGPRRd_bRdData28;
        case 29: return top->io_pTrace_pGPRRd_bRdData29;
        case 30: return top->io_pTrace_pGPRRd_bRdData30;
        case 31: return top->io_pTrace_pGPRRd_bRdData31;
        default: return top->io_pTrace_pGPRRd_bRdData0;
    }
}

word_t getISAGPRData(const char *gpr, bool *success) {
    word_t val = 0;

    for (int i = 0; i < ARRLEN(gpr_name_arr); i++) {
        if (strcmp(gpr_name_arr[i], gpr) == 0) {
            val = getISAGPR(i);
            *success = true;
            break;
        }
    }

    return val;
}

const char *getISAGPRName(int id) {
    return gpr_name_arr[checkISAGPRIsValid(id)];
}

void printfISAGPRData() {
    char *space_num = (char *)"";
    char *space_gpr = (char *)"";
    char *exist_str = (char *)"";
    for (int i = 0; i < ARRLEN(gpr_name_arr); i++) {
        space_num = (i < 10) ? (char *)" " : (char *)"";
        space_gpr = (strcmp(gpr_name_arr[i], "s10") != 0 &&
                     strcmp(gpr_name_arr[i], "s11") != 0) ? (char *)" " :
                                                            (char *)"";
        exist_str = (getISAGPR(i) != 0) ? (char *)ANSI_FMT("*", ANSI_FG_GREEN) :
                                          (char *)"";
        LOG_PURE("[sdb] [gpr] i: %d%s val: %s%s = " FMT_WORD "%s",
                 i,
                 space_num,
                 space_gpr,
                 gpr_name_arr[i],
                 getISAGPR(i),
                 exist_str);
    }
    LOG_PURE();
}
