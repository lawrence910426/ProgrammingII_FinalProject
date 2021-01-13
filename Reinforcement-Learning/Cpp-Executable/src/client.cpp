#include "client.h"
#include "globals.h"

using namespace Constants;


Client::Client(char *host, int port) : is_master(false) {
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        ;
    // Convert addr
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &addr.sin_addr) < 0)
        ;
    // Connect
    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
        ;
}

Client::Client(int fd) : master_fd(fd), is_master(true) {}

Client::~Client() {
    close(sock);
}

void Client::Stop() {
    running = false;
    if (!is_master) {
        close(master_fd);
        delete this;
    }
}

bool Client::recvFromServer() {
    bzero(&buffer, sizeof(buffer));
    int bytes_cnt = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_cnt <= 0) {
        close(sock);
        return false;
    }

    HandleMessage(buffer);
    return true;
}

bool Client::handle() {
    FD_ZERO(&active_fds);
    FD_SET(sock, &active_fds);

    int sel = select(sock + 1, &active_fds, nullptr, nullptr, nullptr);
    if (sel < 0 && errno != EINTR) {
        return false;
    } else {
        if (!recvFromServer()) {
            // Disconnected
            return false;
        }
    }
    return true;
}

void Client::sendToServer(char *msg) const {
    if (is_master)
        ;
    else
        send(sock, msg, strlen(msg), 0);
    // TODO: send all
}

void Client::SendRegister(std::string &name) {
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::REGISTER);
    message[1] = char(name.length());
    strncpy(message + 2, name.c_str(), name.length());
    sendToServer(message);
}

void Client::SendAttack(int target, int lines) {
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::ATTACK);
    message[1] = char(target);
    message[2] = char(lines);
    sendToServer(message);
}

void Client::SendDead() {
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::DEATH);
    sendToServer(message);
}

int perm[9] = {0, 1, 7, 6, 3, 4, 5, 8, 2};
void Client::SendUpdateBoard(std::vector<std::vector<int> > board) {
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::UPDATE_BOARD);
    int p = 1;
    for (int i = TILE_COUNT_V - 1; i >= 0; i--) {
        for (int j = 0; j < TILE_COUNT_H; j++) {
            message[p++] = char(perm[board[i][j]]);
        }
    }
    sendToServer(message);
}

void Client::HandleMessage(char *msg) {
    const auto op = HiyaOperation(msg[0]);

    if (op == HiyaOperation::REGISTER) {
        id = int(msg[1]);
    } else if (op == HiyaOperation::START) {
        should_start_game = true;
    }
}
