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

extern "C" uint32_t readSimInstData(uint32_t addr, uint8_t len) {
    uint32_t data = (uint32_t)readMemoryPhyData(addr, len);
    top->io_pTrace_pMemInst_pRd_bData = data;

#ifdef CONFIG_MTRACE_PROCESS
    printfDebugMTrace((char *)"process", (char *)"rd ins", addr, data, 0);
#endif

    return data;
}

extern "C" uint32_t readSimMemoryData(uint32_t addr, uint8_t len) {
    uint32_t data = (uint32_t)readMemoryPhyData(addr, len);
    top->io_pTrace_pMemData_pRd_bData = data;

#ifdef CONFIG_MTRACE_PROCESS
    printfDebugMTrace((char *)"process", (char *)"rd mem", addr, data, 0);
#endif

    return data;
}

extern "C" void writeSimMemoryData(uint32_t addr, uint32_t data, uint8_t len) {
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

#ifdef CONFIG_DEVICE
    uint64_t mem_rd_addr = top->io_pTrace_pMemData_pRd_bAddr;
    uint64_t mem_wr_addr = top->io_pTrace_pMemData_pWr_bAddr;
    if ((mem_rd_addr & 0xfffffff8) == CONFIG_TIMER_MMIO    ||
        (mem_rd_addr & 0xffffffff) == CONFIG_KEYBOARD_MMIO ||
        (mem_rd_addr & 0xffffffff) == CONFIG_VGA_CTL_MMIO  ||
        (mem_wr_addr & 0xffffffff) == CONFIG_SERIAL_MMIO   ||
        (mem_wr_addr & 0xfffffff0) == CONFIG_VGA_CTL_MMIO  ||
        (mem_wr_addr & 0xff000000) == CONFIG_VGA_FB_ADDR) {
        IFDEF(CONFIG_DIFFTEST, skipDebugDifftestRef());
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
bool     sim_end_pre_flag = false;
bool     sim_end_all_flag = false;

void runCPUSimModule() {
    if (!sim_end_all_flag) {
        sim_pc   = top->io_pTrace_pBase_bPC;
        sim_snpc = sim_pc + 4;
        sim_inst = top->io_pTrace_pBase_bInst;

        runCPUSimModuleCycle(true);

        sim_dnpc = top->io_pTrace_pBase_bPC;
        sim_cycle_num++;

        sim_end_pre_flag = top->io_pState_bEndPreFlag;
        sim_end_all_flag = top->io_pState_bEndAllFlag;

#ifdef CONFIG_ETRACE
#ifdef CONFIG_ETRACE_PROCESS
        printfDebugETrace((char *)"process", top->io_pState_bCSRType);
#endif
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

    if (sim_end_all_flag) {
        sim_cycle_num--;
        setCPUState(CPU_END, sim_pc, top->io_pState_bEndAllData);
    }
}

void resetCPUSimModule(int num) {
    top->reset = 1;
    while (num-- > 0) {
        runCPUSimModuleCycle(false);
    }
    top->reset = 0;
}
