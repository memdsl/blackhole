#ifndef LOG_H
#define LOG_H

#include <common.hpp>

#define PRINT_LOG(type, format, ...)                                       \
    do {                                                                   \
        bool color_flag = false;                                           \
        auto color_fg   = fmt::fg(fmt::color::white);                      \
        if (strcmp(type, "success") == 0) {                                \
            color_flag = true;                                             \
            color_fg   = fmt::fg(fmt::color::green);                       \
        }                                                                  \
        else if (strcmp(type, "warning") == 0) {                           \
            color_flag = true;                                             \
            color_fg   = fmt::fg(fmt::color::orange);                      \
        }                                                                  \
        else if (strcmp(type, "failed") == 0) {                            \
            color_flag = true;                                             \
            color_fg   = fmt::fg(fmt::color::red);                         \
        }                                                                  \
        if (color_flag) {                                                  \
            MUX_DEF(CONFIG_DEBUG_LOG_STDOUT, fmt::print(color_fg,          \
                                                        format,            \
                                                        ## __VA_ARGS__),); \
        }                                                                  \
        else {                                                             \
            MUX_DEF(CONFIG_DEBUG_LOG_STDOUT, fmt::print(format,            \
                                                        ## __VA_ARGS__),); \
        }                                                                  \
    } while (0)

#define PRINT_LOG_ASSERT(cond, format, ...)              \
    do {                                                 \
        if (!cond) {                                     \
            PRINT_LOG("failed", format, ## __VA_ARGS__); \
            assert(cond);                                \
        }                                                \
    } while(0)

#define PRINT_LOG_PANICS(format, ...)                \
    do {                                             \
        PRINT_LOG_ASSERT(0, format, ## __VA_ARGS__); \
    } while (0)


class Log {
public:
    Log();
    ~Log();
public:
    void initLog(const string log_file);
private:
    FILE *log_file_p;
};

#endif
