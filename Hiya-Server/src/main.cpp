#include "server.h"

int main() {
    for (;;) {
        auto server = new Server(7122);

        while (server->running)
            server->handle();

        delete server;
    }
}