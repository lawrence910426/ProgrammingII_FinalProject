#include "server.h"

int main() {
    auto server = new Server(7122);
    for (;;) {
        server->handle();
    }
}