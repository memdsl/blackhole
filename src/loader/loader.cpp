#include <loader.hpp>

Loader::Loader() {
    g_log_ptr = make_shared<Log>();
    g_mem_ptr = make_shared<Memory>(g_log_ptr);
}

Loader::~Loader() {
}

void Loader::initLoader(int argc, char *argv[]) {
    parseLoaderArgs(argc, argv);

    g_log_ptr->initLog(g_log_file);
    g_mem_ptr->initMemory();

    printLoaderWelcome();
}

void Loader::handleLoaderImage() {

}

int Loader::parseLoaderArgs(int argc, char *argv[]) {
    CLI::App app{"Blackhole Runtime Environment"};
    argv = app.ensure_utf8(argv);

    g_bch = false;
    app.add_option("-b,--bch", g_bch, "Enable batch mode of environment");

    g_img_file = "";
    app.add_option("-i,--img", g_img_file, "Set the path of software img file");

    g_log_file = "";
    app.add_option("-l,--log", g_log_file, "Set the path of software log file");

    g_ref_file = "";
    app.add_option("-r,--ref", g_ref_file, "Set the path of difftest ref file");

    CLI11_PARSE(app, argc, argv);

    return 0;
}

void Loader::printLoaderWelcome() {
    Time time;

    g_log_ptr->printLog("", "[loader] [cfgs] [debug] log:               ", "");
    MUX_DEF(CONFIG_DEBUG_LOG, g_log_ptr->printLog("success", "{}\n", "ON"),
                              g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug]   log_stdout:      ", "");
    MUX_DEF(CONFIG_DEBUG_LOG_STDOUT, g_log_ptr->printLog("success", "{}\n", "ON"),
                                     g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug]   log_file:        ", "");
    MUX_DEF(CONFIG_DEBUG_LOG_FILE, g_log_ptr->printLog("success", "{}\n", "ON"),
                                   g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug] ldb:               ", "");
    MUX_DEF(CONFIG_DEBUG_LDB, g_log_ptr->printLog("success", "{}\n", "ON"),
                              g_log_ptr->printLog("failed",  "{}\n", "OFF"));\

    g_log_ptr->printLog("", "[loader] [cfgs] [debug] trace:             ", "");
    MUX_DEF(CONFIG_DEBUG_TRACE, g_log_ptr->printLog("success", "{}\n", "ON"),
                                g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug]   trace_ins:       ", "");
    MUX_DEF(CONFIG_DEBUG_TRACE_INS, g_log_ptr->printLog("success", "{}\n", "ON"),
                                    g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug]     trace_ins_pro: ", "");
    MUX_DEF(CONFIG_DEBUG_TRACE_INS_PROCESS, g_log_ptr->printLog("success", "{}\n", "ON"),
                                            g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug]     trace_ins_res: ", "");
    MUX_DEF(CONFIG_DEBUG_TRACE_INS_RESULT, g_log_ptr->printLog("success", "{}\n", "ON"),
                                           g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug]   trace_mem:       ", "");
    MUX_DEF(CONFIG_DEBUG_TRACE_MEM, g_log_ptr->printLog("success", "{}\n", "ON"),
                                    g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug]     trace_mem_pro: ", "");
    MUX_DEF(CONFIG_DEBUG_TRACE_MEM_PROCESS, g_log_ptr->printLog("success", "{}\n", "ON"),
                                            g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug]     trace_mem_res: ", "");
    MUX_DEF(CONFIG_DEBUG_TRACE_MEM_RESULT, g_log_ptr->printLog("success", "{}\n", "ON"),
                                           g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug] diff:              ", "");
    MUX_DEF(CONFIG_DEBUG_DIFF, g_log_ptr->printLog("success", "{}\n", "ON"),
                               g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug] wave:              ", "");
    MUX_DEF(CONFIG_DEBUG_WAVE, g_log_ptr->printLog("success", "{}\n", "ON"),
                               g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [cfgs] [debug]   wave_cycle:      ", "");
    MUX_DEF(CONFIG_DEBUG_WAVE, g_log_ptr->printLog("success", "{}\n", CONFIG_DEBUG_WAVE_CYCLE),
                               g_log_ptr->printLog("failed",  "{}\n", "OFF"));

    g_log_ptr->printLog("", "[loader] [time] ", "");
    g_log_ptr->printLog("failed", "{}\n", time.getTimeNowDateAndTime().c_str());
    g_log_ptr->printLog("", "[loader] [info] Welcome to ", "");
    g_log_ptr->printLog("warning", "BLACKHOLE!\n", "");
}
