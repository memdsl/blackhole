#include <loader.hpp>

Loader::Loader() {
    Log log;
}

Loader::~Loader() {
}

void Loader::initLoader(int argc, char *argv[]) {
    parseLoaderArgs(argc, argv);

    log.initLog(log_file);

    printLoaderWelcome();
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
    MUX_DEF(CONFIG_DEBUG_LOG, log.printLog("success", "{}\n", "ON"),
                              log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug]  log_stdout:       ");
    MUX_DEF(CONFIG_DEBUG_LOG_STDOUT, log.printLog("success", "{}\n", "ON"),
                                     log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug]  log_file:         ");
    MUX_DEF(CONFIG_DEBUG_LOG_FILE, log.printLog("success", "{}\n", "ON"),
                                   log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug] ldb:               ");
    MUX_DEF(CONFIG_DEBUG_LDB, log.printLog("success", "{}\n", "ON"),
                              log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug] trace:             ");
    MUX_DEF(CONFIG_DEBUG_TRACE, log.printLog("success", "{}\n", "ON"),
                                log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug]   trace_ins:       ");
    MUX_DEF(CONFIG_DEBUG_TRACE_INS, log.printLog("success", "{}\n", "ON"),
                                    log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug]     trace_ins_pro: ");
    MUX_DEF(CONFIG_DEBUG_TRACE_INS_PROCESS, log.printLog("success", "{}\n", "ON"),
                                            log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug]     trace_ins_res: ");
    MUX_DEF(CONFIG_DEBUG_TRACE_INS_RESULT, log.printLog("success", "{}\n", "ON"),
                                           log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug]   trace_mem:       ");
    MUX_DEF(CONFIG_DEBUG_TRACE_MEM, log.printLog("success", "{}\n", "ON"),
                                    log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug]     trace_mem_pro: ");
    MUX_DEF(CONFIG_DEBUG_TRACE_MEM_PROCESS, log.printLog("success", "{}\n", "ON"),
                                            log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug]     trace_mem_res: ");
    MUX_DEF(CONFIG_DEBUG_TRACE_MEM_RESULT, log.printLog("success", "{}\n", "ON"),
                                           log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug] diff:              ");
    MUX_DEF(CONFIG_DEBUG_DIFF, log.printLog("success", "{}\n", "ON"),
                               log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug] wave:              ");
    MUX_DEF(CONFIG_DEBUG_WAVE, log.printLog("success", "{}\n", "ON"),
                               log.printLog("failed",  "{}\n", "OFF"));
    fmt::print("[loader] [cfgs] [debug]   wave_cycle:      ");
    MUX_DEF(CONFIG_DEBUG_WAVE, log.printLog("success", "{}\n", CONFIG_DEBUG_WAVE_CYCLE),
                               log.printLog("failed",  "{}\n", "OFF"));

    fmt::print("[loader] [time] ");
    fmt::print(fg(fmt::color::red), "{}\n", time.getTimeNowDateAndTime());
    fmt::print("[loader] [info] Welcome to ");
    fmt::print(fg(fmt::color::orange), "BLACKHOLE!\n");
}
