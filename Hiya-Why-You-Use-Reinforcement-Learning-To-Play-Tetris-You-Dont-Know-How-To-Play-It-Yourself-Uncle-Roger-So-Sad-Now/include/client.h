//#pragma once
//
//#include <allegro5/allegro5.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <netinet/in.h>
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <unistd.h>
//
//#include <algorithm>
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
//#include <iostream>
//#include <map>
//#include <set>
//
//#include "game.h"
//
//#define BUFFER_SIZE 5000
//
//class Client {
// public:
//    Client(char *host, int port, Game &game);
////    Client(int fd, Game &game);
////    ~Client();
//
//    void handle();
//
//    void HandleMessage(char *msg);
//
//    void SendRegister(std::string &name);
//    void SendUpdateBoard(char *board_encoding);
//    void SendAttack(int target, int lines);
//    void SendDead();
//
//    int master_fd = -1;
//
//    int id = -1;
//
//    int sock;
//    bool is_master;
//
//    std::map<int, std::string> players;
//
// private:
//    std::set<int> client_fds;
//
//    sockaddr_in addr{};
//
//    fd_set fds{};
//    fd_set active_fds{};
//
//    char buffer[BUFFER_SIZE]{};
//    char message[BUFFER_SIZE]{};
//
//
//    bool recvFromServer();
//
//    void sendToServer(char *message) const;
//
//    Game &game;
//};