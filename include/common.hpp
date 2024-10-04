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

#endif
