#include <sys/time.h>

#include <common.h>
#include <cpu/sim.h>
#include <debug/trace.h>
#include <device/keyboard.h>
#include <device/vga.h>
#include <memory/memory.h>
#include <state.h>
#include <util/log.h>
#include <util/timer.h>

typedef unsigned int       uint32_tt;
typedef unsigned long long uint64_tt;

bool sim_ebreak = false;

extern "C" void judgeIsEbreak(uint8_t inst_end_flag) {
    sim_ebreak = inst_end_flag;
}

extern "C" uint32_tt readInsData(uint64_tt addr, uint8_t len) {
    uint32_tt data = 0;
    if (addr != 0x00000000) {
        data = (uint32_tt)readPhyMemData(addr, len);
#ifdef CONFIG_MTRACE_COND_PROCESS
        printfDebugMTrace((char *)"process", (char *)"rd ins", addr, data, 0);
#endif
    }
    return data;
}

extern "C" uint64_tt readMemData(uint64_tt addr, uint8_t len) {
    uint64_tt data = 0;
    if (addr == 0xa0000048) {
        data = (uint64_tt)getTimerValue();
    }
    else if (addr == 0xa0000060) {
        data = (uint64_tt)dequeueDiviceKey();
    }
    else if (addr == 0xa0000100) {
        data = (uint64_tt)(getDeviceVGAScreenWidth() << 16 |
                           getDeviceVGAScreenHeight());
    }
    else if (judgeAddrIsInPhyMem(addr)) {
        data = (uint64_tt)readPhyMemData(addr, len);
    }
    else {
        data = 0;
    }
#ifdef CONFIG_MTRACE_COND_PROCESS
    printfDebugMTrace((char *)"process", (char *)"rd mem", addr, data, 0);
#endif
    return data;
}

extern "C" void writeMemData(uint64_tt addr, uint64_tt data, uint8_t len) {
    if (addr == 0xa00003f8) {
        putc((uint8_t)data, stderr);
    }
    else {
        writePhyMemData(addr, len, data);
    }
#ifdef CONFIG_MTRACE_COND_PROCESS
    printfDebugMTrace((char *)"process", (char *)"wr mem", addr, data, 0);
#endif
}

static bool inst_func_call = false;
static bool inst_func_ret  = false;

VerilatedContext *contextp = NULL;
#ifdef CONFIG_DEBUG_WAVE
VerilatedVcdC    *tfp = NULL;
#endif
VTop             *top = NULL;

static void runCPUSimStep() {
    top->eval();
#ifdef CONFIG_DEBUG_WAVE
    contextp->timeInc(1);
    tfp->dump(contextp->time());
#endif
}

static void runCPUSimModuleCycle(bool flag) {
    top->clock = 0;
    runCPUSimStep();

#ifdef CONFIG_ITRACE_COND_PROCESS
    if (flag) {
        printfDebugITrace("process");
    }
#endif

    top->clock = 1;
    runCPUSimStep();
}

void initCPUSim() {
    contextp = new VerilatedContext;
#ifdef CONFIG_DEBUG_WAVE
    tfp = new VerilatedVcdC;
#endif
    top = new VTop;
    contextp->traceEverOn(true);
#ifdef CONFIG_DEBUG_WAVE
    top->trace(tfp, 0);
    tfp->open("build/cpu/wave.vcd");
#endif
}

void exitCPUSim() {
    runCPUSimStep();
#ifdef CONFIG_DEBUG_WAVE
    tfp->close();
#endif
    delete top;
}

uint64_t sim_pc   = 0;
uint64_t sim_snpc = 0;
uint64_t sim_dnpc = 0;
uint64_t sim_inst = 0;
uint64_t sim_cycle_num = 1;

void runCPUSimModule(bool *inst_end_flag) {
    if (!sim_ebreak) {
        sim_pc   = top->io_pTrace_pBase_bPC;
        sim_snpc = sim_pc + 4;
        sim_inst = top->io_pTrace_pBase_bInst;

        top->io_pTrace_pMem_bRdDataA = readInsData(
            top->io_pTrace_pMem_bRdAddrA,
            4);
        top->io_pTrace_pMem_bRdDataB = readMemData(
            top->io_pTrace_pMem_bRdAddrB,
            4);

        if (top->io_pTrace_pMem_bWrEn) {
            uint8_t len = 4;
            bool wr_mask_0 = top->io_pTrace_pMem_bWrMask_0;
            bool wr_mask_1 = top->io_pTrace_pMem_bWrMask_1;
            bool wr_mask_2 = top->io_pTrace_pMem_bWrMask_2;
            bool wr_mask_3 = top->io_pTrace_pMem_bWrMask_3;
            if (!wr_mask_3 && !wr_mask_2 && !wr_mask_1 && wr_mask_0) {
                len = 1;
            }
            else if (!wr_mask_3 && !wr_mask_2 && wr_mask_1 && wr_mask_0) {
                len = 2;
            }
            else {
                len = 4;
            }
            writeMemData(top->io_pTrace_pMem_bWrAddr,
                         top->io_pTrace_pMem_bWrData,
                         len);
        }

        runCPUSimModuleCycle(true);

        sim_dnpc = top->io_pTrace_pBase_bPC;
        sim_cycle_num++;

        if (top->io_pEnd_bFlag) {
            sim_ebreak = true;
        }

#if CFLAGS_CPU_TYPE_ME32LS
        *inst_end_flag = true;
#elif CFLAGS_CPU_TYPE_ME32LM
#elif CFLAGS_CPU_TYPE_ME32LP
#endif

#ifdef CONFIG_FTRACE
        bool inst_func_call = top->io_pTrace_pBase_bInstCall;
        bool inst_func_ret  = top->io_pTrace_pBase_bInstRet;
#ifdef CONFIG_FTRACE_COND_PROCESS
        printfDebugFTrace((char *)"process",
                          inst_func_call,
                          inst_func_ret,
                          sim_pc,
                          sim_dnpc);
#else
        printfDebugFTrace((char *)"",
                          inst_func_call,
                          inst_func_ret,
                          sim_pc,
                          sim_dnpc);
#endif
#endif
    }

    if (sim_ebreak) {
        sim_cycle_num--;
        setNPCState(NPC_END, sim_pc, top->io_pEnd_bData);
    }
}

void resetCPUSimModule(int num) {
    top->reset = 1;
    while (num-- > 0) {
        runCPUSimModuleCycle(false);
    }
    top->reset = 0;
}
