#pragma once

#include <allegro5/allegro5.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <set>

#include "game.h"

class Game;

class Server {
 public:
    Server(int port, Game &game);
    ~Server();

    void handle();
    bool running = true;

    void HandleMessage(int fd, char *msg);

    void ResponseRegister(int fd);
    void SendPlayers();
    void SendGameStart();
    void SendUpdateBoard(int fd, char *board_encoding);
    void SendAttack(int attacker, int target, int lines);
    void SendPlayerDead(int fd);

    bool started = false;

    int master_fd;

 private:
    std::set<int> client_fds;

    std::map<int, std::string> players;

    sockaddr_in addr{};

    fd_set fds{};
    fd_set active_fds{};

    char buffer[BUFFER_SIZE]{};
    char message[BUFFER_SIZE]{};

    void handleNewConnection();

    // false if client disconnected
    bool recvFromClient(int fd);
    void disconnectClient(int fd);

    static void sendTo(int fd, char *messageBuffer);
    void sendToAllOther(int source_fd, char *message);

    Game &game;
};