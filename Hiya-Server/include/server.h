#pragma once


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <set>
#include <algorithm>

#define BUFFER_SIZE 1000

class Server
{
 public:
    explicit Server(int port);
    ~Server();

    void handle();

 private:

    std::set<int> client_fds;

    sockaddr_in addr;

    fd_set fds;
    fd_set active_fds;

    int master_fd;

    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    void handleNewConnection();

    // false if client disconnected
    bool recvFromClient(int fd);

    static void sendTo(int fd, char *messageBuffer);
    void sendToAllOther(int source_fd, char *message);

};