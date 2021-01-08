#include "client.h"
#include "log.h"

#include "globals.h"

#include "game.h"

using namespace Constants;


Client::Client(char *host, int port, Game &game) : is_master(false), game(game) {

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        FATAL("Socket creation failed!")

    // Convert addr
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &addr.sin_addr) < 0)
        FATAL("Address convert failed!")

    // Connect
    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
        FATAL("Connection failed!")

    INFO("Client connected to " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port))
}

Client::Client(int fd, Game &game) : master_fd(fd), is_master(true), game(game) {}

Client::~Client() {
    INFO("Client stopping...")
    close(sock);
}

void Client::Stop() {
    running = false;
}

bool Client::recvFromServer() {
    bzero(&buffer, sizeof(buffer));
    int bytes_cnt = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_cnt <= 0) {
        if (bytes_cnt != 0)
            WARN("Server recv failed: " << bytes_cnt)
        close(sock);
        INFO("Server disconnected!")

        return false;
    }

    HandleMessage(buffer);
    return true;
}

void Client::handle() {
    FD_ZERO(&active_fds);
    FD_SET(sock, &active_fds);

    INFO("Waiting for activity...")
    int sel = select(sock + 1, &active_fds, nullptr, nullptr, nullptr);
    if (sel < 0 && errno != EINTR) {
        WARN("Select failed!")
        FATAL("Server may be disconnected!")
    } else {
        if (!recvFromServer()) {
            // Disconnected
            WARN("Server is disconnected!")
        }
    }
}

void Client::sendToServer(char *msg) const {
    if (is_master)
        game.server->HandleMessage(master_fd, msg);
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
    INFO("Sending attack to " << target << " with " << lines << "lines")
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::ATTACK);
    message[1] = char(target);
    message[2] = char(lines);
    sendToServer(message);
}

void Client::SendDead() {
    INFO("Sending player death message")
    bzero(message, sizeof(message));
    message[0] = char(HiyaOperation::DEATH);
    sendToServer(message);
}

void Client::SendUpdateBoard(char *board_encoding) {
//    INFO("Sending updated board")
//    bzero(message, sizeof(message));
//    message[0] = char(HiyaOperation::UPDATE_BOARD);
//    strncpy(message + 1, board_encoding, TILE_COUNT_H * TILE_COUNT_V);
//    sendToServer(fd, message);
}

void Client::HandleMessage(char *msg) {
    INFO("Handling operation")
    const auto op = HiyaOperation(msg[0]);

    if (op == HiyaOperation::REGISTER) {
        id = int(msg[1]);
        INFO("Registed with id " << id)
    } else if (op == HiyaOperation::PLAYERS) {
        const int cnt = int(msg[1]);
        int i = 2;
        for (int k = 0; k < cnt; k++) {
            const int fd = int(msg[i++]);
            const int len = int(msg[i++]);
            char name[len + 10];
            strncpy(name, msg + i, len);
            i += len;
            name[len] = '\0';
            players.emplace(std::make_pair(fd, std::string(name)));
            INFO("Current Players: " << name);
        }
    } else if (op == HiyaOperation::START) {
        game.status = GameStatus::PLAYING;
    } else if (op == HiyaOperation::UPDATE_BOARD) {
    } else if (op == HiyaOperation::ATTACK) {
    } else if (op == HiyaOperation::DEATH) {
    } else {
        WARN("Operation " << int(op) << " not recognized")
    }
}
