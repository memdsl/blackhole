#ifndef COMMON_H
#define COMMON_H

#include <autoconf.h>
#include <util/time.hpp>

#define CONCAT(x, y) x ## y

#define MUX_DEF_0 X,
#define MUX_DEF_1 X,
#define MUX_CHOOSE_2ND(a, b, ...) b
#define MUX_JUDGE_COMMA(mux_comma, a, b) MUX_CHOOSE_2ND(mux_comma a, b)
#define MUX_JUDGE(mux_def, macro, a, b) MUX_JUDGE_COMMA(CONCAT(mux_def, macro), a, b)
#define MUX(macro, x, y)  MUX_JUDGE(MUX_DEF_, macro, x, y)

#endif
