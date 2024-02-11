#include <sys/time.h>

#include <common.h>
#include <cpu/sim.h>
#include <debug/difftest.h>
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

extern "C" uint32_tt readInsData(uint32_tt addr, uint8_t len) {
    uint32_tt data = 0;
    // addr = addr & 0xfffffffc;
    // addr = addr & ~0x3u;

    if (addr != 0x00000000) {
        data = (uint32_tt)readMemoryPhyData(addr, len);
#ifdef CONFIG_MTRACE_PROCESS
        printfDebugMTrace((char *)"process", (char *)"rd ins", addr, data, 0);
#endif
    }

    top->io_pTrace_pMem_bRdDataA = data;
    return data;
}

extern "C" uint32_tt readMemData(uint32_tt addr, uint8_t len) {
    // uint32_tt data = 0;
    // addr = addr & 0xfffffffc;
    // addr = addr & ~0x3u;

    uint32_tt data = (uint32_tt)readMemoryPhyData(addr, len);
#ifdef CONFIG_MTRACE_PROCESS
    printfDebugMTrace((char *)"process", (char *)"rd mem", addr, data, 0);
#endif

    top->io_pTrace_pMem_bRdDataB = data;
    return data;
}

extern "C" void writeMemData(uint32_tt addr, uint32_tt data, uint8_t len) {
    // addr = addr & 0xfffffffc;
    // addr = addr & ~0x3u;

    writeMemoryPhyData(addr, len, data);
#ifdef CONFIG_MTRACE_PROCESS
    printfDebugMTrace((char *)"process", (char *)"wr mem", addr, data, 0);
#endif
}

static bool inst_func_call = false;
static bool inst_func_ret  = false;

VerilatedContext *contextp = NULL;
VTop *top = NULL;
#ifdef CONFIG_DEBUG_WAVE
VerilatedVcdC *tfp = NULL;
#endif

static void runCPUSimStep() {
    top->eval();
#ifdef CONFIG_DEBUG_WAVE
    tfp->dump(contextp->time());
    contextp->timeInc(1);
#endif
}

static void runCPUSimModuleCycle(bool flag) {
    top->clock = 0;
    runCPUSimStep();

#ifdef CONFIG_ITRACE_PROCESS
    if (flag) {
        printfDebugITrace((char *)"process");
    }
#endif

    top->clock = 1;
    runCPUSimStep();
}

void initCPUSim() {
    contextp = new VerilatedContext;
    contextp->debug(0);
    contextp->traceEverOn(true);

    top = new VTop{contextp};

#ifdef CONFIG_DEBUG_WAVE
    tfp = new VerilatedVcdC;
    top->trace(tfp, 5);
    tfp->open("cpu/build/wave.vcd");
#endif
}

void exitCPUSim() {
    runCPUSimStep();
    top->final();
    delete top;
#ifdef CONFIG_DEBUG_WAVE
    tfp->close();
    delete tfp;
#endif
}

uint64_t sim_pc   = 0;
uint64_t sim_snpc = 0;
uint64_t sim_dnpc = 0;
uint64_t sim_inst = 0;
uint64_t sim_cycle_num = 1;
bool     sim_inst_end_flag = false;

void runCPUSimModule() {
    if (!sim_ebreak) {
        sim_pc   = top->io_pTrace_pBase_bPC;
        sim_snpc = sim_pc + 4;
        sim_inst = top->io_pTrace_pBase_bInst;

        runCPUSimModuleCycle(true);

        sim_dnpc = top->io_pTrace_pBase_bPC;
        sim_cycle_num++;

        if (top->io_pEnd_bFlag) {
            sim_ebreak = true;
        }

#if CFLAGS_CPU_TYPE_ME32LS
        sim_inst_end_flag = true;
#elif CFLAGS_CPU_TYPE_ME32LM
#elif CFLAGS_CPU_TYPE_ME32LP
#endif

#ifdef CONFIG_FTRACE
        bool inst_func_call = top->io_pTrace_pBase_bInstCall;
        bool inst_func_ret  = top->io_pTrace_pBase_bInstRet;
#ifdef CONFIG_FTRACE_PROCESS
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
        setCPUState(CPU_END, sim_pc, top->io_pEnd_bData);
    }
}

void resetCPUSimModule(int num) {
    top->reset = 1;
    while (num-- > 0) {
        runCPUSimModuleCycle(false);
    }
    top->reset = 0;
}
