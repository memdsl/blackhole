#ifndef LOADER_H
#define LOADER_H

#include <common.hpp>
#include <log.hpp>

class Loader {
public:
    Loader();
    ~Loader();
public:
    void initLoader(int argc, char *argv[]);
private:
    void handleLoaderImage();
    int parseLoaderArgs(int argc, char *argv[]);
    void printLoaderWelcome();
private:
    bool   bch;
    string img_file;
    string log_file;
    string ref_file;
    Log    log;
};

#endif
