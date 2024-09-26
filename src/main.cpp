#include <loader.h>
#include <status.h>

using namespace std;

int main(int argc, char *argv[]) {
    Loader loader;
    loader.initLoader(argc, argv);

    Status status;
    status.getStatus();
    return 0;
}