#ifndef LOADER_H
#define LOADER_H

class Loader {
public:
    Loader();
    ~Loader();
public:
    void initLoader(int argc, char *argv[]);
private:
    void handleLoaderImage();
    void parseLoaderArgv(int argc, char *argv[]);
    void printLoaderWelcome();
private:
};

#endif
