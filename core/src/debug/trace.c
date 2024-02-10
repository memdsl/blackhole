#include <elf.h>

#include <cpu/sim.h>
#include <debug/trace.h>
#include <memory/memory.h>

#define ITRACE_BUF_LEN 16
char  *itrace[ITRACE_BUF_LEN];
char **itrace_head = NULL;
char **itrace_tail = itrace + (ITRACE_BUF_LEN - 1);
char **itrace_curr = NULL;

extern VTop *top;
extern uint64_t sim_cycle_num;

void recordDebugITrace(char *logbuf) {
    if (itrace_head == NULL) {
        itrace_head = itrace;
    }
    else {
        if (itrace_head != itrace_tail) {
            itrace_head++;
        }
        else {
            itrace_head = itrace;
        }
    }

    if (*itrace_head == NULL) {
        *itrace_head = (char *)malloc(sizeof(char) * 256);
    }
    strcpy(*itrace_head, logbuf);
    itrace_curr = itrace_head;
}

void printfDebugITrace(char *type) {
    if (strcmp(type, "process") == 0) {
        LOG_BRIEF("[itrace] cycle num:                 %ld", sim_cycle_num);
        LOG_BRIEF("[itrace] [base]       pc:           " FMT_WORD,
                  top->io_pTrace_pBase_bPC);
        LOG_BRIEF("[itrace] [base]       inst:         " FMT_WORD,
                  top->io_pTrace_pBase_bInst);

        LOG_BRIEF("[itrace] [gpr] [wr]   wr en:        %d",
                  top->io_pTrace_pGPRWr_bWrEn);
        LOG_BRIEF("[itrace] [gpr] [wr]   wr addr:      " FMT_WORD,
                  top->io_pTrace_pGPRWr_bWrAddr);
        LOG_BRIEF("[itrace] [gpr] [wr]   wr data:      " FMT_WORD,
                  top->io_pTrace_pGPRWr_bWrData);

        LOG_BRIEF("[itrace] [mem]        rd en:        %d",
                  top->io_pTrace_pMem_bRdEn);
        LOG_BRIEF("[itrace] [mem]        rd addr a:    " FMT_WORD,
                  top->io_pTrace_pMem_bRdAddrA);
        LOG_BRIEF("[itrace] [mem]        rd addr b:    " FMT_WORD,
                  top->io_pTrace_pMem_bRdAddrB);
        LOG_BRIEF("[itrace] [mem]        wr en:        %d",
                  top->io_pTrace_pMem_bWrEn);
        LOG_BRIEF("[itrace] [mem]        wr addr:      " FMT_WORD,
                  top->io_pTrace_pMem_bWrAddr);
        LOG_BRIEF("[itrace] [mem]        wr data:      " FMT_WORD,
                  top->io_pTrace_pMem_bWrData);
        LOG_BRIEF("[itrace] [mem]        wr mask:      %d %d %d %d",
                  top->io_pTrace_pMem_bWrMask_0,
                  top->io_pTrace_pMem_bWrMask_1,
                  top->io_pTrace_pMem_bWrMask_2,
                  top->io_pTrace_pMem_bWrMask_3);
        LOG_BRIEF("[itrace] [mem]        rd data a:    " FMT_WORD,
                  top->io_pTrace_pMem_bRdDataA);
        LOG_BRIEF("[itrace] [mem]        rd data b:    " FMT_WORD,
                  top->io_pTrace_pMem_bRdDataB);

        char *inst_name = (char *)"";
        switch (top->io_pTrace_pIDUCtr_bInstName) {
            case  0: inst_name = (char *)"X     "; break;
            case  1: inst_name = (char *)"SLL   "; break;
            case  2: inst_name = (char *)"SLLI  "; break;
            case  3: inst_name = (char *)"SRL   "; break;
            case  4: inst_name = (char *)"SRLI  "; break;
            case  5: inst_name = (char *)"SRA   "; break;
            case  6: inst_name = (char *)"SRAI  "; break;
            case  7: inst_name = (char *)"ADD   "; break;
            case  8: inst_name = (char *)"ADDI  "; break;
            case  9: inst_name = (char *)"SUB   "; break;
            case 10: inst_name = (char *)"LUI   "; break;
            case 11: inst_name = (char *)"AUIPC "; break;
            case 12: inst_name = (char *)"XOR   "; break;
            case 13: inst_name = (char *)"XORI  "; break;
            case 14: inst_name = (char *)"OR    "; break;
            case 15: inst_name = (char *)"ORI   "; break;
            case 16: inst_name = (char *)"AND   "; break;
            case 17: inst_name = (char *)"ANDI  "; break;
            case 18: inst_name = (char *)"SLT   "; break;
            case 19: inst_name = (char *)"SLTI  "; break;
            case 20: inst_name = (char *)"SLTU  "; break;
            case 21: inst_name = (char *)"SLTIU "; break;
            case 22: inst_name = (char *)"BEQ   "; break;
            case 23: inst_name = (char *)"BNE   "; break;
            case 24: inst_name = (char *)"BLT   "; break;
            case 25: inst_name = (char *)"BGE   "; break;
            case 26: inst_name = (char *)"BLTU  "; break;
            case 27: inst_name = (char *)"BGEU  "; break;
            case 28: inst_name = (char *)"JAL   "; break;
            case 29: inst_name = (char *)"JALR  "; break;
            case 30: inst_name = (char *)"FENCE "; break;
            case 31: inst_name = (char *)"FENCEI"; break;
            case 32: inst_name = (char *)"ECALL "; break;
            case 33: inst_name = (char *)"EBREAK"; break;
            case 34: inst_name = (char *)"CSRRW "; break;
            case 35: inst_name = (char *)"CSRRS "; break;
            case 36: inst_name = (char *)"CSRRC "; break;
            case 37: inst_name = (char *)"CSRRWI"; break;
            case 38: inst_name = (char *)"CSRRSI"; break;
            case 39: inst_name = (char *)"CSRRCI"; break;
            case 40: inst_name = (char *)"LB    "; break;
            case 41: inst_name = (char *)"LH    "; break;
            case 42: inst_name = (char *)"LBU   "; break;
            case 43: inst_name = (char *)"LHU   "; break;
            case 44: inst_name = (char *)"LW    "; break;
            case 45: inst_name = (char *)"SB    "; break;
            case 46: inst_name = (char *)"SH    "; break;
            case 47: inst_name = (char *)"SW    "; break;
            case 48: inst_name = (char *)"MUL   "; break;
            case 49: inst_name = (char *)"MULH  "; break;
            case 50: inst_name = (char *)"MULHSU"; break;
            case 51: inst_name = (char *)"MULHU "; break;
            case 52: inst_name = (char *)"DIV   "; break;
            case 53: inst_name = (char *)"DIVU  "; break;
            case 54: inst_name = (char *)"REM   "; break;
            case 55: inst_name = (char *)"REMU  "; break;
            default: inst_name = (char *)"X";      break;
        }
        LOG_BRIEF("[itrace] [idu] [ctr]  inst name:    %s ", inst_name);

        char *alu_type = (char *)"";
        switch (top->io_pTrace_pIDUCtr_bALUType) {
            case  0: alu_type = (char *)"X     "; break;
            case  1: alu_type = (char *)"SLL   "; break;
            case  2: alu_type = (char *)"SRL   "; break;
            case  3: alu_type = (char *)"SRA   "; break;
            case  4: alu_type = (char *)"ADD   "; break;
            case  5: alu_type = (char *)"SUB   "; break;
            case  6: alu_type = (char *)"XOR   "; break;
            case  7: alu_type = (char *)"OR    "; break;
            case  8: alu_type = (char *)"AND   "; break;
            case  9: alu_type = (char *)"SLT   "; break;
            case 10: alu_type = (char *)"SLTU  "; break;
            case 11: alu_type = (char *)"BEQ   "; break;
            case 12: alu_type = (char *)"BNE   "; break;
            case 13: alu_type = (char *)"BLT   "; break;
            case 14: alu_type = (char *)"BGE   "; break;
            case 15: alu_type = (char *)"BLTU  "; break;
            case 16: alu_type = (char *)"BGEU  "; break;
            case 17: alu_type = (char *)"JALR  "; break;
            case 18: alu_type = (char *)"MUL   "; break;
            case 19: alu_type = (char *)"MULH  "; break;
            case 20: alu_type = (char *)"MULHSU"; break;
            case 21: alu_type = (char *)"MULHU "; break;
            case 22: alu_type = (char *)"DIV   "; break;
            case 23: alu_type = (char *)"DIVU  "; break;
            case 24: alu_type = (char *)"REM   "; break;
            case 25: alu_type = (char *)"REMU  "; break;
            case 26: alu_type = (char *)"SLLW  "; break;
            case 27: alu_type = (char *)"SRLW  "; break;
            case 28: alu_type = (char *)"SRLIW "; break;
            case 29: alu_type = (char *)"SRAW  "; break;
            case 30: alu_type = (char *)"SRAIW "; break;
            case 31: alu_type = (char *)"DIVW  "; break;
            case 32: alu_type = (char *)"DIVUW "; break;
            case 33: alu_type = (char *)"REMW  "; break;
            default: alu_type = (char *)"X     "; break;
        }
        LOG_BRIEF("[itrace] [idu] [ctr]  alu type:     %s", alu_type);

        char *alu_rs1 = (char *)"";
        switch (top->io_pTrace_pIDUCtr_bALURS1) {
            case 0:  alu_rs1 = (char *)"X";   break;
            case 1:  alu_rs1 = (char *)"PC";  break;
            case 2:  alu_rs1 = (char *)"GPR"; break;
            default: alu_rs1 = (char *)"X";   break;
        }
        LOG_BRIEF("[itrace] [idu] [ctr]  alu rs1:      %s", alu_rs1);

        char *alu_rs2 = (char *)"";
        switch (top->io_pTrace_pIDUCtr_bALURS2) {
            case 0:  alu_rs2 = (char *)"X";     break;
            case 1:  alu_rs2 = (char *)"GPR";   break;
            case 2:  alu_rs2 = (char *)"CSR";   break;
            case 3:  alu_rs2 = (char *)"IMM_I"; break;
            case 4:  alu_rs2 = (char *)"IMM_S"; break;
            case 5:  alu_rs2 = (char *)"IMM_B"; break;
            case 6:  alu_rs2 = (char *)"IMM_U"; break;
            case 7:  alu_rs2 = (char *)"IMM_J"; break;
            case 8:  alu_rs2 = (char *)"4";     break;
            default: alu_rs2 = (char *)"X";     break;
        }
        LOG_BRIEF("[itrace] [idu] [ctr]  alu rs2:      %s", alu_rs2);

        LOG_BRIEF("[itrace] [idu] [ctr]  jmp en:       %d",
                  top->io_pTrace_pIDUCtr_bJmpEn);
        LOG_BRIEF("[itrace] [idu] [ctr]  mem wr en:    %d",
                  top->io_pTrace_pIDUCtr_bMemWrEn);

        char *mem_byt = (char *)"";
        switch (top->io_pTrace_pIDUCtr_bMemByt) {
            case 0:  mem_byt = (char *)"X";   break;
            case 1:  mem_byt = (char *)"1_U"; break;
            case 2:  mem_byt = (char *)"2_U"; break;
            case 3:  mem_byt = (char *)"4_U"; break;
            case 4:  mem_byt = (char *)"8_U"; break;
            case 5:  mem_byt = (char *)"1_S"; break;
            case 6:  mem_byt = (char *)"2_S"; break;
            case 7:  mem_byt = (char *)"4_S"; break;
            case 8:  mem_byt = (char *)"8_S"; break;
            default: mem_byt = (char *)"X";   break;
        }
        LOG_BRIEF("[itrace] [idu] [ctr]  mem byt:      %s", mem_byt);

        char *gpr_wr_src = (char *)"";
        switch (top->io_pTrace_pIDUCtr_bGPRWrSrc) {
            case 0:  gpr_wr_src = (char *)"X";   break;
            case 1:  gpr_wr_src = (char *)"ALU"; break;
            case 2:  gpr_wr_src = (char *)"MEM"; break;
            case 3:  gpr_wr_src = (char *)"PC";  break;
            case 4:  gpr_wr_src = (char *)"CSR"; break;
            default: gpr_wr_src = (char *)"X";   break;
        }
        LOG_BRIEF("[itrace] [idu] [ctr]  gpr wr src:   %s", gpr_wr_src);

        LOG_BRIEF("[itrace] [idu] [ctr]  gpr wr en:    %d",
                  top->io_pTrace_pIDUCtr_bGPRWrEn);

        LOG_BRIEF("[itrace] [idu] [data] gpr rd addr:  " FMT_WORD,
                  top->io_pTrace_pIDUData_bGPRRdAddr);
        LOG_BRIEF("[itrace] [idu] [data] alu rs1 data: " FMT_WORD,
                  top->io_pTrace_pIDUData_bALURS1Data);
        LOG_BRIEF("[itrace] [idu] [data] alu rs2 data: " FMT_WORD,
                  top->io_pTrace_pIDUData_bALURS2Data);
        LOG_BRIEF("[itrace] [idu] [data] jmp/wr data:  " FMT_WORD,
                  top->io_pTrace_pIDUData_bJmpOrWrData);

        LOG_BRIEF("[itrace] [exu] [out]  alu out:      " FMT_WORD,
                  top->io_pTrace_pEXUOut_bALUOut);

        LOG_BRIEF();
    }
    else if (strcmp(type, "result") == 0) {
        itrace_head = itrace;
        while (*itrace_head != NULL && itrace_head != itrace_tail) {
            if (itrace_head == itrace_curr) {
                LOG_BRIEF("[itrace] ----> %s", *itrace_head);
            }
            else {
                LOG_BRIEF("[itrace]       %s", *itrace_head);
            }
            char *itrace_temp = *itrace_head;
            free(itrace_temp);
            itrace_head++;
        }
    }
}

void printfDebugMTrace(char *type,
                       char *dir,
                       word_t addr,
                       word_t data,
                       word_t len) {
    if (strcmp(type, "process") == 0) {
        LOG_BRIEF("[mtrace] addr: " FMT_WORD " data: " FMT_WORD " %s", addr,
                                                                       data,
                                                                       dir);
    }
    else if (strcmp(type, "result") == 0) {
        word_t addr_base = (addr != 0) ? addr : CONFIG_MBASE;
        for (word_t i = 0; i < len; i++) {
            addr = addr_base + i * 4;
            data = readPhyMemData(addr, 4);
            LOG_BRIEF("[mtrace] addr: " FMT_WORD " data: " FMT_WORD, addr,
                                                                     data);
        }
    }
}

#define ARR_LEN 1024 * 1024 * 10

static int    inst_func_call_depth = -1;
static char  *inst_func_name_arr[ARR_LEN];
static char **inst_func_name_head = inst_func_name_arr;
static char  *func_name_arr[ARR_LEN];

static int judgeDebugFTraceIsELF32(FILE *fp) {
    char buf[16];
    int  nread = fread(buf, 1, 16, fp);
    fseek(fp, 0, SEEK_SET);

    if (nread < 16) {
        return 0;
    }
    if (strncmp(buf, ELFMAG, SELFMAG)) {
        return 0;
    }
    if (buf[EI_CLASS] != ELFCLASS32) {
        return 0;
    }

    return 1;
}

static char *getDebugFTraceFunc(Elf32_Addr addr) {
    Elf32_Addr offset = addr - CONFIG_MBASE;
    ASSERT(offset < ARR_LEN, "[ftrace] out of bounds: %d", offset);
    if (func_name_arr[offset] != NULL) {
        return func_name_arr[offset];
    }
    else {
        return (char *)"";
    }
    }

static void initDebugFTrace(char *elf_file) {
    if (elf_file != NULL) {
        FILE *fp = fopen(elf_file, "r");
        ASSERT(fp, "[ftrace] can not open '%s'", elf_file);
        ASSERT(judgeDebugFTraceIsELF32(fp), "[ftrace] file type mismatch");

        Elf32_Ehdr elf_header;
        fread(&elf_header, 1, sizeof(elf_header), fp);

        Elf32_Shdr elf_section_arr[elf_header.e_shnum];
        fseek(fp, elf_header.e_shoff, SEEK_SET);
        fread(elf_section_arr,
              sizeof(Elf32_Shdr),
              (elf_header.e_shnum * elf_header.e_shentsize),
              fp);

        Elf32_Shdr elf_section_symbol;
        Elf32_Shdr elf_section_string;
        for (int i = 0; i < elf_header.e_shnum; i++) {
            if (elf_section_arr[i].sh_type == SHT_SYMTAB) {
                memcpy(&elf_section_symbol,
                       &elf_section_arr[i],
                       sizeof(Elf32_Shdr));
                if (elf_section_arr[elf_section_symbol.sh_link].sh_type ==
                    SHT_STRTAB) {
                    memcpy(&elf_section_string,
                           &elf_section_arr[elf_section_symbol.sh_link],
                           sizeof(Elf32_Shdr));
                }
            }
        }

        char elf_string_name_arr[elf_section_string.sh_size];
        fseek(fp, elf_section_string.sh_offset, SEEK_SET);
        fread(&elf_string_name_arr, 1, sizeof(elf_string_name_arr), fp);

        Elf32_Xword elf_symbol_cnt = elf_section_symbol.sh_size /
                                     elf_section_symbol.sh_entsize;
        Elf32_Sym elf_symbol_arr[elf_symbol_cnt];
        fseek(fp, elf_section_symbol.sh_offset, SEEK_SET);
        fread(&elf_symbol_arr, 1, elf_section_symbol.sh_size, fp);
        for (int i = 0; i < elf_symbol_cnt; i++) {
            uint8_t st_info = elf_symbol_arr[i].st_info;
            if (ELF64_ST_TYPE(st_info) == STT_FUNC) {
                Elf32_Addr st_value = elf_symbol_arr[i].st_value;
                Elf32_Word st_name = elf_symbol_arr[i].st_name;
                char *func_name = elf_string_name_arr + st_name;
                Elf32_Addr offset = st_value - elf_header.e_entry;
                ASSERT(offset < ARR_LEN, "[trace] out of bounds: %d", offset);
                if (func_name_arr[offset] == NULL) {
                    func_name_arr[offset] = (char *)malloc(sizeof(char) * 256);
                }
                strcpy(func_name_arr[offset], func_name);
#ifdef CONFIG_FTRACE_ELF
                printf("[ftrace] symb addr: " FMT_WORD "\n", st_value);
                printf("[ftrace] func name:  %s\n\n", func_name);
#endif
            }
        }

        fclose(fp);
    }
}

char  *ftrace_info_arr[ARR_LEN];
char **ftrace_info_head = ftrace_info_arr;

void printfDebugFTrace(char *type,
                       bool inst_func_call,
                       bool inst_func_ret,
                       word_t pc,
                       word_t dpnc) {
    if (strcmp(type, "result") == 0) {
        ftrace_info_head = ftrace_info_arr;
        while (*ftrace_info_head != NULL) {
            printf("%s", *ftrace_info_head);
            char *ftrace_info_temp = *ftrace_info_head;
            free(ftrace_info_temp);
            ftrace_info_head++;
        }
        return;
    }

    char buf[256];
    char buf_head[128];
    char buf_tail[128];

    if (inst_func_call || inst_func_ret) {
        sprintf(buf_head, "[ftrace] addr: 0x%08" PRIx32, (uint32_t)pc);
    }

    if (inst_func_call) {
        inst_func_name_head++;
        inst_func_call_depth++;
        if (*inst_func_name_head == NULL) {
            *inst_func_name_head = (char *)malloc(sizeof(char *) * 256);
        }
        strcpy(*inst_func_name_head, getDebugFTraceFunc(dpnc));
        char printf_format[] = " call %*s[%s@" "0x%08" PRIx32 "]\n";
        sprintf(buf_tail,
                printf_format,
                inst_func_call_depth * 2,
                "",
                *inst_func_name_head,
                (uint32_t)dpnc);
    }

    if (inst_func_ret) {
        inst_func_name_head--;
        inst_func_call_depth--;
        char printf_format[] = " ret  %*s[%s]\n";
        sprintf(buf_tail,
                printf_format,
                inst_func_call_depth * 2,
                "",
                *inst_func_name_head);
    }

    if (inst_func_call || inst_func_ret) {
        sprintf(buf, "%s%s", buf_head, buf_tail);
        if (*ftrace_info_head == NULL) {
            *ftrace_info_head = (char *)malloc(sizeof(char *) * 256);
        }
        strcpy(*ftrace_info_head, buf);
        ftrace_info_head++;

        if (strcmp(type, "process") == 0) {
            printf("%s", buf);
        }
    }
}

void freeDebugFTrace() {
    for (int i = 0; i < ARR_LEN; i++) {
        free(func_name_arr[i]);
    }
}

void printfDebugDTrace(char *type,
                       char *dir,
                       const char *name,
                       word_t addr,
                       word_t data) {
    if (strcmp(type, "process") == 0) {
        printf("[dtrace] addr: " FMT_WORD " data: " FMT_WORD " %s %s\n", addr,
                                                                         data,
                                                                         dir,
                                                                         name);
    }
    else if (strcmp(type, "result") == 0) {
    }
}

void initDebugTrace(char *elf_file) {
    initDebugFTrace(elf_file);
}
