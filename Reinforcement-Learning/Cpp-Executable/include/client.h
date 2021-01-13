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

#include "globals.h"
using namespace Constants;

class Client {
 public:
    Client(char *host, int port);
    Client(int fd);
    ~Client();

    bool handle();
    bool running = true;
    void Stop();

    void HandleMessage(char *msg);

    void SendRegister(std::string &name);
    void SendUpdateBoard(std::vector<std::vector<int> > board);
    void SendAttack(int target, int lines);
    void SendDead();

    int master_fd = -1;

    int id = -1;

    int sock{};
    bool is_master;

    std::vector<int> player_list;
    std::set<int> players_alive;

    bool should_start_game = false;

private:
    std::set<int> client_fds;

    sockaddr_in addr{};

    fd_set fds{};
    fd_set active_fds{};

    char buffer[BUFFER_SIZE]{};
    char message[BUFFER_SIZE]{};


    bool recvFromServer();

    void sendToServer(char *message) const;
};