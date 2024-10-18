#ifndef COMMON_H
#define COMMON_H

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/ranges.h>

#include <CLI11.hpp>

#include <autoconf.h>
#include <time.hpp>

using namespace std;

#define CONCAT(x, y) x ## y

#define MUX_DEF_0 X,
#define MUX_DEF_1 X,
#define MUX_DEF_CHOOSE_2ND(a, b, ...) b
#define MUX_DEF_JUDGE_COMMA(mux_comma, a, b) MUX_DEF_CHOOSE_2ND(mux_comma a, b)
#define MUX_DEF_JUDGE(mux_def, macro, a, b) MUX_DEF_JUDGE_COMMA(CONCAT(mux_def, macro), a, b)
#define MUX_DEF(macro, x, y)  MUX_DEF_JUDGE(MUX_DEF_, macro, x, y)

#define MUX_CON(con, x, y) \
    if (con) {             \
        x;                 \
    }                      \
    else {                 \
        y;                 \
    }                      \

typedef MUX_DEF(CONFIG_BASE_CPU_ISA_RISCV32, uint32_t, uint64_t) u_data_t;
typedef MUX_DEF(CONFIG_BASE_CPU_ISA_RISCV32,  int32_t,  int64_t) s_data_t;

#if CONFIG_MEMORY_BASE + CONFIG_MEMORY_SIZE > 0x100000000ul
    #define MEM_ADDR_WIDTH_64 1
#endif
typedef MUX_DEF(MEM_ADDR_WIDTH_64, uint64_t, uint32_t) u_addr_t;

#endif
