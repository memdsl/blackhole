#ifndef LOG_H
#define LOG_H

#include <common.hpp>

class Log {
public:
    Log();
    ~Log();
public:
    void initLog(const string log_file);
    template<typename args_0, typename... args_1_to_n>
    void printLog(const string type, const args_0 args_fmt,
                  const args_1_to_n... args_var);
    template<typename args_0, typename... args_1_to_n>
    void printLogAssert(const bool flag, const args_0 args_fmt,
                        const args_1_to_n... args_var);
    template<typename args_0, typename... args_1_to_n>
    void printLogPanic(const args_0 args_fmt, const args_1_to_n... args_var);
private:
    FILE *log_file_p;
};

#endif
