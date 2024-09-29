#include <loader.hpp>
#include <status.hpp>

using namespace std;

int main(int argc, char *argv[]) {
    Loader loader;
    loader.initLoader(argc, argv);

    Status status;
    return status.getStatus();
}
