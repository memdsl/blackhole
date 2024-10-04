#include <log.hpp>

Log::Log() {
}

Log::~Log() {
}

void Log::initLog(const string log_file) {
    if (!log_file.empty()) {
        FILE *log_file_p_tmp = fopen(log_file.c_str(), "w");
        assert(log_file_p_tmp);
        log_file_p = log_file_p_tmp;
    }
    printLog("success", "[loader] [init] [log] {}\n", "finished");
}

template<typename args_0, typename... args_1_to_n>
void Log::printLog(const string type, const args_0 args_fmt,
                   const args_1_to_n... args_var) {
    bool args_color_flag = false;
    auto args_color = fmt::fg(fmt::color::white);

    if (type.compare("success") == 0) {
        args_color_flag = true;
        args_color = fmt::fg(fmt::color::green);
    }
    else if (type.compare("warning") == 0) {
        args_color_flag = true;
        args_color = fmt::fg(fmt::color::orange);
    }
    else if (type.compare("failed") == 0) {
        args_color_flag = true;
        args_color = fmt::fg(fmt::color::red);
    }

    MUX_CON(
        args_color_flag,
        MUX_DEF(CONFIG_DEBUG_LOG_STDOUT, fmt::print(args_color,
                                                    args_fmt,
                                                    args_var...),),
        MUX_DEF(CONFIG_DEBUG_LOG_STDOUT, fmt::print(args_fmt,
                                                    args_var...),)
    );

    MUX_DEF(CONFIG_DEBUG_LOG_FILE, fmt::print(log_file_p,
                                              args_fmt,
                                              args_var...),);
}
