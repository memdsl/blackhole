#ifndef LOADER_H
#define LOADER_H

#include <common.hpp>
#include <log.hpp>
#include <memory.hpp>

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
    bool               g_bch;
    string             g_img_file;
    string             g_log_file;
    string             g_ref_file;
    shared_ptr<Log>    g_log_ptr;
    shared_ptr<Memory> g_mem_ptr;
};

#endif
