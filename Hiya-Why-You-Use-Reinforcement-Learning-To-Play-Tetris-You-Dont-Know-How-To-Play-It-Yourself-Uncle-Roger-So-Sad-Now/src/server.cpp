#include "server.h"
#include "log.h"

#include "globals.h"

using namespace Constants;


Server::Server(int port, Game &game): game(game) {

    // Create socket
    master_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (master_fd < 0)
        FATAL("Socket creation failed!")

    FD_ZERO(&fds);
    FD_SET(master_fd, &fds);

    // Set options
    int opt_val = 1;
    if (setsockopt(master_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) < 0)
        FATAL("Socket set options failed!")

    // Bind
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(master_fd, (sockaddr *) &addr, sizeof(addr)) < 0)
        FATAL("Socket binding failed!")

    // Listen
    if (listen(master_fd, 3) < 0)
        FATAL("Socket listen failed!")

    INFO("Server listening on " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port))
}

Server::~Server() {
    INFO("Server stopping...")
    for (int fd: client_fds)
        close(fd);
    close(master_fd);
}

void Server::Stop() {
    running = false;
}

void Server::handleNewConnection() {
    INFO("Handling new connection...")
    socklen_t addrlen = sizeof(addr);
    int client_fd = accept(master_fd, (struct sockaddr *) &addr, &addrlen);

    if (client_fd < 0) {
        WARN("Connection failed to accept: " << client_fd)
    } else {
        FD_SET(client_fd, &fds);
        client_fds.emplace(client_fd);
        INFO("New Connection: fd:" << client_fd << " Address:" << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port))
    }
}

void Server::disconnectClient(int fd) {
    close(fd);
    FD_CLR(fd, &fds);
    INFO("Client " << fd << " disconnected!")
    players.erase(fd);
}

bool Server::recvFromClient(int fd) {
    bzero(&buffer, sizeof(buffer));
//    int bytes_cnt = 0, buftop = 0;
//    do {
//        bytes_cnt = recv(fd, buffer + buftop, sizeof(buffer) - buftop, 0);
//        if (bytes_cnt < 0) {
//            WARN("Client " << fd << " recv failed: " << bytes_cnt)
//            disconnectClient(fd);
//            return false;
//        }
//        buftop += bytes_cnt;
//        INFO(bytes_cnt);
//    } while (bytes_cnt > 0);
//
//    if (buftop == 0) {
//        disconnectClient(fd);
//        return false;
//    }

    int bytes_cnt = recv(fd, buffer, sizeof(buffer), 0);
    if (bytes_cnt <= 0) {
        if (bytes_cnt != 0)
            WARN("Client " << fd << " recv failed: " << bytes_cnt)
        disconnectClient(fd);
        return false;
    }


    INFO("Receive from client " << fd)

    HandleMessage(fd, buffer);
    return true;
}

void Server::handle() {
    active_fds = fds;

    INFO("Waiting for activity...")
    const int nfds = (client_fds.empty()? master_fd: std::max(*client_fds.rbegin(), master_fd)) + 1;
    int sel = select(nfds, &active_fds, nullptr, nullptr, nullptr);
    if (sel < 0 && errno != EINTR)
        WARN("Select failed!")
    else {
        // Master socket is active: handle new connection
        if (FD_ISSET(master_fd, &active_fds))
            handleNewConnection();

        for (auto it = client_fds.begin(); it != client_fds.end();) {
            if (FD_ISSET(*it, &active_fds)) {
                // Client is active: receive data
                if (!recvFromClient(*it)) {
                    // Client disconnected
                    it = client_fds.erase(it);
                    if (started)
                        SendPlayerDead(*it);
                    else
                        SendPlayers();
                    continue;
                }
            }
            it++;
        }
    }
}

void Server::sendTo(int fd, char *msg) {
    INFO("Sending to " << fd)
    if (fd == master_fd)
        game.client->HandleMessage(msg);
    else {
        int msglen = strlen(msg);
        int sent = 0;
        while (sent < msglen) {
            int bytes_sent = send(fd, msg + sent, msglen - sent, 0);
            sent += bytes_sent;
        }
    }
}

void Server::sendToAllOther(int source_fd, char *msg) {
    for (int fd: client_fds) {
        INFO("Remaning FDs " << fd)
        if (fd != source_fd)
            sendTo(fd, msg);
    }
    if (source_fd != master_fd)
         game.client->HandleMessage(msg);
}

void Server::SendGameStart() {
    started = true;
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::START);
    sendToAllOther(0, message);
}

void Server::ResponseRegister(int fd) {
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::REGISTER);
    message[1] = char(fd);
    sendTo(fd, message);
    if (!started)
        SendPlayers();
}

void Server::SendAttack(int attacker, int target, int lines) {
    INFO("Attack from " << attacker << " to " << target << " with " << lines << "lines")
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::ATTACK);
    message[1] = char(attacker);
    message[2] = char(target);
    message[3] = char(lines);
    sendToAllOther(attacker, message);
}

void Server::SendPlayerDead(int fd) {
    INFO("Player " << fd << " is Dead")
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::DEATH);
    message[1] = char(fd);
    sendToAllOther(fd, message);
}

void Server::SendUpdateBoard(int fd, char *board_encoding) {
    INFO("Player " << fd << " updates board")
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::UPDATE_BOARD);
    message[1] = char(fd);
    strncpy(message + 2, board_encoding, TILE_COUNT_H * TILE_COUNT_V);
    sendToAllOther(fd, message);
}

void Server::SendPlayers() {
    INFO("Sending players")
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::PLAYERS);
    message[1] = char(players.size());
    int i = 2;
    for (auto &[id, name]: players) {
        message[i++] = char(id);
        message[i++] = name.length();
        strncpy(message + i, name.c_str(), name.length());
        i += name.length();
    }
    sendToAllOther(0, message);
}

void Server::HandleMessage(int fd, char *msg) {
    INFO("Handling operation from " << fd)
    const auto op = HiyaOperation(msg[0]);

    if (op == HiyaOperation::REGISTER) {
        if (started) {
            ResponseRegister(-1);
            return;
        }
        const int len = int(msg[1]);
        char name[260];
        strncpy(name, msg + 2, len);
        players.emplace(make_pair(fd, std::string(name)));
        INFO("Player " << fd << " registered with name " << name)
        ResponseRegister(fd);
    } else if (op == HiyaOperation::UPDATE_BOARD) {
        SendUpdateBoard(fd, msg + 1);
    } else if (op == HiyaOperation::ATTACK) {
        SendAttack(fd, int(msg[1]), int(msg[2]));
    } else if (op == HiyaOperation::DEATH) {
        SendPlayerDead(fd);
    } else {
        WARN("Operation " << int(op) << " not recognized")
    }
}

