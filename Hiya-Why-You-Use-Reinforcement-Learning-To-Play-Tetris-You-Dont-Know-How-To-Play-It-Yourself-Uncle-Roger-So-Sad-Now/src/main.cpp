#include "window.h"
#include "globals.h"

int main(int argc, char *argv[]) {
    auto *window= new Window();

    window->Start();

    delete window;
    return 0;
}
