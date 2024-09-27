#ifndef TIME_H
#define TIME_H

#include <ctime>
#include <string>

using namespace std;

class Time {
public:
    Time();
    ~Time();
public:
    string getTimeNowDateAndTime();
};

#endif
