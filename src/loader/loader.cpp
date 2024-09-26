#include <ctime>
#include <fmt/color.h>

#include <loader.h>
#include <btime.h>

Loader::Loader() {
}

Loader::~Loader() {
}

void Loader::initLoader(int argc, char *argv[]) {

    this->printLoaderWelcome();
}

void Loader::handleLoaderImage() {

}

void Loader::parseLoaderArgv(int argc, char *argv[]) {

}

void Loader::printLoaderWelcome() {
    Time time;

    fmt::print("[loader] cfgs: \n");
    fmt::print("[loader] cfgs: \n");
    fmt::print("[loader] cfgs: \n");
    fmt::print("[loader] cfgs: \n");
    fmt::print("[loader] cfgs: \n");
    fmt::print("[loader] cfgs: \n");
    fmt::print("[loader] time: ");
    fmt::print(fg(fmt::color::red), "{}\n", time.getTimeNowDateAndTime());
    fmt::print("[loader] info: Welcome to ");
    fmt::print(fg(fmt::color::orange), "BLACKHOLE!\n");
}
