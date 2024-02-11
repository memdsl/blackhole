#include <locale.h>

#include <cpu/cpu.h>
#include <cpu/sim.h>
#include <debug/difftest.h>
#include <debug/trace.h>
#include <device/device.h>
#include <isa/isa.h>
#include <isa/gpr.h>
#include <monitor/sdb/watch.h>
#include <state.h>
#include <util/disasm.h>
#include <util/log.h>
#include <util/timer.h>

#define MAX_INST_TO_PRINT 0

static uint64_t cpu_timer = 0;
static bool cpu_print_step = true;

extern uint64_t sim_pc;
extern uint64_t sim_snpc;
extern uint64_t sim_dnpc;
extern uint64_t sim_inst;
extern uint64_t sim_cycle_num;
extern bool sim_inst_end_flag;

char cpu_logbuf[256];
uint64_t cpu_inst_num = 0;
CPUStruct cpu = {};

static void execCPUTraceAndDifftest() {
#ifdef CONFIG_ITRACE_PROCESS
    writeLog("%s\n", cpu_logbuf);
#endif

#ifdef CONFIG_ITRACE_RESULT
    recordDebugITrace(cpu_logbuf);
#endif

#ifdef CONFIG_SDB_WATCH
    if (traceSDBWatch() > 0) {
        cpu_state.state = CPU_STOP;
    }
#endif

    if (cpu_print_step) {
        IFDEF(CONFIG_ITRACE, puts(cpu_logbuf));
    }
    IFDEF(CONFIG_DIFFTEST, stepDebugDifftest(sim_pc, sim_dnpc));
}

static void execCPUTimesSingle() {
    runCPUSimModule();
    cpu.pc = sim_dnpc;

    if (sim_inst_end_flag) {
        cpu_inst_num++;
        sim_inst_end_flag = false;
    }

#ifdef CONFIG_ITRACE_RESULT
        char *cpu_logbuf_p = cpu_logbuf;
        cpu_logbuf_p += snprintf(cpu_logbuf_p,
                                 sizeof(cpu_logbuf),
                                 FMT_WORD,
                                 sim_pc);
        int ilen = sim_snpc - sim_pc;
        int i;
        uint8_t *inst = (uint8_t *)&sim_inst;
        for (i = ilen - 1; i >= 0; i--) {
            cpu_logbuf_p += snprintf(cpu_logbuf_p, 4, " %02x", inst[i]);
        }
        int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
        int space_len = ilen_max - ilen;
        if (space_len < 0) space_len = 0;
        space_len = space_len * 3 + 1;
        memset(cpu_logbuf_p, ' ', space_len);
        cpu_logbuf_p += space_len;

        execDisasm(cpu_logbuf_p,
                   cpu_logbuf + sizeof(cpu_logbuf) - cpu_logbuf_p,
                   MUXDEF(CONFIG_ISA_x86, sim_snpc, sim_pc),
                         (uint8_t *)&sim_inst, ilen);

#endif
}

static void execCPUTimesMultip(uint64_t num) {
    for (; num > 0; num--) {
        execCPUTimesSingle();
        execCPUTraceAndDifftest();
        if (cpu_state.state != CPU_RUNNING) {
            break;
        }
        IFDEF(CONFIG_DEVICE, updateDeviceState());
    }
}

static void printfCPUStatistic() {
    IFNDEF(CONFIG_TARGET_AM, setlocale(LC_NUMERIC, ""));
    #define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%", "%'") PRIu64
    LOG_BRIEF_COLOR("[statistic] time      = " NUMBERIC_FMT " us", cpu_timer);
    LOG_BRIEF_COLOR("[statistic] freq      = %lf KHz", (1.0 / cpu_timer) * 1000);
    LOG_BRIEF_COLOR("[statistic] inst  num = " NUMBERIC_FMT, cpu_inst_num);
    LOG_BRIEF_COLOR("[statistic] cycle num = " NUMBERIC_FMT, sim_cycle_num);
    if (cpu_timer > 0) {
        LOG_BRIEF_COLOR(
            "[statistic] sim freq  = " NUMBERIC_FMT " inst/s",
            cpu_inst_num * 1000000 / cpu_timer);
    }
    else {
        LOG_BRIEF_COLOR("[statistic] sim freq  = 0 inst/s (time < 1us, can't " \
                        "calculate sim freq)");
    }
    LOG_BRIEF_COLOR("[statistic] ipc       = %lf inst/c",
                    (double)cpu_inst_num / sim_cycle_num);
    LOG_BRIEF_COLOR("[statistic] cpi       = %lf c/inst",
                    (double)sim_cycle_num / cpu_inst_num);
}

void execCPU(uint64_t num) {
    cpu_print_step = (num < MAX_INST_TO_PRINT);
    switch (cpu_state.state) {
        case CPU_END: case CPU_ABORT:
            printf("Program execution has ended. To restart the program, " \
                   "exit CPU and run again.\n");
            return;
        default: cpu_state.state = CPU_RUNNING;
    }

    uint64_t timer_start = getTimerValue();

    execCPUTimesMultip(num);

    uint64_t timer_end = getTimerValue();
    cpu_timer += timer_end - timer_start;

    switch (cpu_state.state) {
        case CPU_RUNNING: { cpu_state.state = CPU_STOP; break; }
        case CPU_END: case CPU_ABORT: {
#ifdef CONFIG_ITRACE_RESULT
#ifndef CONFIG_ITRACE_PROCESS
            LOG_BRIEF();
#endif
            printfDebugITrace((char *)"result");
#endif
#ifdef CONFIG_MTRACE_RESULT
            LOG_BRIEF();
            printfDebugMTrace((char *)"result", NULL, 0, 0, 16);
#endif
#ifdef CONFIG_FTRACE_RESULT
            LOG_BRIEF();
            printfDebugFTrace((char *)"result", NULL, NULL, 0, 0);
#endif
            LOG_BRIEF();
            LOG_BRIEF_COLOR("[result] state: %s",
                (cpu_state.state == CPU_ABORT ?
                    ANSI_FMT("ABORT", ANSI_FG_RED) :
                    (cpu_state.halt_ret == 0 ?
                        ANSI_FMT("SUCCESS", ANSI_FG_GREEN) :
                        ANSI_FMT("FAILED", ANSI_FG_RED))));
            LOG_BRIEF_COLOR("[result] pc:    " FMT_WORD, cpu_state.halt_pc);
            LOG_BRIEF_COLOR();
        }
        case CPU_QUIT: { printfCPUStatistic(); }
    }
}

void printfCPUAssertFailMsg() {
    printfISAGPRData();
    printfCPUStatistic();
}
