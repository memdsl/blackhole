#include <fmt/color.h>

#include <common.hpp>
#include <loader.hpp>

Loader::Loader() {
}

Loader::~Loader() {
}

void Loader::initLoader(int argc, char *argv[]) {

    this->printLoaderWelcome();
}

void Loader::handleLoaderImage() {

}

void Loader::parseLoaderArgv(int argc, char *argv[]) {

}

void Loader::printLoaderWelcome() {
    Time time;

    fmt::print("[loader] [cfgs] [debug] log:               ");
    MUX(CONFIG_DEBUG_LOG, fmt::print(fg(fmt::color::green), "ON\n"),
                          fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug] ldb:               ");
    MUX(CONFIG_DEBUG_LDB, fmt::print(fg(fmt::color::green), "ON\n"),
                          fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug] trace:             ");
    MUX(CONFIG_DEBUG_TRACE, fmt::print(fg(fmt::color::green), "ON\n"),
                            fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug]   trace_ins:       ");
    MUX(CONFIG_DEBUG_TRACE_INS, fmt::print(fg(fmt::color::green), "ON\n"),
                                fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug]     trace_ins_pro: ");
    MUX(CONFIG_DEBUG_TRACE_INS_PROCESS, fmt::print(fg(fmt::color::green), "ON\n"),
                                        fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug]     trace_ins_res: ");
    MUX(CONFIG_DEBUG_TRACE_INS_RESULT, fmt::print(fg(fmt::color::green), "ON\n"),
                                       fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug]   trace_mem:       ");
    MUX(CONFIG_DEBUG_TRACE_MEM, fmt::print(fg(fmt::color::green), "ON\n"),
                                fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug]     trace_mem_pro: ");
    MUX(CONFIG_DEBUG_TRACE_MEM_PROCESS, fmt::print(fg(fmt::color::green), "ON\n"),
                                        fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug]     trace_mem_res: ");
    MUX(CONFIG_DEBUG_TRACE_MEM_RESULT, fmt::print(fg(fmt::color::green), "ON\n"),
                                       fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug] diff:              ");
    MUX(CONFIG_DEBUG_DIFF, fmt::print(fg(fmt::color::green), "ON\n"),
                           fmt::print(fg(fmt::color::red),   "OFF\n"));
    fmt::print("[loader] [cfgs] [debug] wave:              ");
    MUX(CONFIG_DEBUG_WAVE, fmt::print(fg(fmt::color::green), "ON\n"),
                           fmt::print(fg(fmt::color::red),   "OFF\n"));

    fmt::print("[loader] [time] ");
    fmt::print(fg(fmt::color::red), "{}\n", time.getTimeNowDateAndTime());
    fmt::print("[loader] [info] Welcome to ");
    fmt::print(fg(fmt::color::orange), "BLACKHOLE!\n");
}
