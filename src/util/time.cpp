#include <time.hpp>

Time::Time() {

}

Time::~Time() {

}

string Time::getTimeNowDateAndTime() {
    time_t time_now = time(0);
    tm    *time_now_local = localtime(&time_now);

    int    time_buf_len = 24;
    char   time_buf[time_buf_len];
    string time_str;

    strftime(time_buf, time_buf_len, "%Y-%m-%d %H:%M:%S", time_now_local);
    time_str = time_buf;

    return time_str;
}
