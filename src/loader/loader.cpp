#include <fmt/color.h>
#include <CLI11.hpp>

#include <common.hpp>
#include <loader.hpp>

Loader::Loader() {
}

Loader::~Loader() {
}

void Loader::initLoader(int argc, char *argv[]) {
    this->parseLoaderArgs(argc, argv);


    this->printLoaderWelcome();
}

void Loader::handleLoaderImage() {

}

int Loader::parseLoaderArgs(int argc, char *argv[]) {
    CLI::App app{"Blackhole Runtime Environment"};
    argv = app.ensure_utf8(argv);

    bch = false;
    app.add_option("-b,--bch", bch, "Enable batch mode of environment");

    img_file = "";
    app.add_option("-i,--img", img_file, "Set the path of software img file");

    log_file = "";
    app.add_option("-l,--log", log_file, "Set the path of software log file");

    ref_file = "";
    app.add_option("-r,--ref", ref_file, "Set the path of difftest ref file");

    CLI11_PARSE(app, argc, argv);

    return 0;
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
    fmt::print("[loader] [cfgs] [debug]   wave_cycle:      ");
    MUX(CONFIG_DEBUG_WAVE, fmt::print(fg(fmt::color::green), "{}\n", CONFIG_DEBUG_WAVE_CYCLE),
                           fmt::print(fg(fmt::color::red),   "OFF\n"));

    fmt::print("[loader] [time] ");
    fmt::print(fg(fmt::color::red), "{}\n", time.getTimeNowDateAndTime());
    fmt::print("[loader] [info] Welcome to ");
    fmt::print(fg(fmt::color::orange), "BLACKHOLE!\n");
}
