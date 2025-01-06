#include <log.hpp>

Log::Log() {
    log_file_p = NULL;
}

Log::~Log() {
}

void Log::initLog(const string log_file) {
    if (!log_file.empty()) {
        FILE *log_file_p_tmp = fopen(log_file.c_str(), "w");
        PRINT_LOG_ASSERT(log_file_p_tmp, "[log] [error] can not open {}\n", log_file);
        log_file_p = log_file_p_tmp;
    }
    PRINT_LOG("success", "[loader] [init] [log] {}\n", "finished");
    PRINT_LOG("success", "[loader] [init] [log] {}\n", log_file);
}
