//#include "client.h"
//#include "log.h"
//
//#include "globals.h"
//
//using namespace Constants;
//
//
//Client::Client(char *host, int port, Game &game) : is_master(false), game(game) {
//
//    // Create socket
//    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (sock < 0)
//        FATAL("Socket creation failed!")
//
//    // Convert addr
//    addr.sin_family = AF_INET;
//    addr.sin_port = htons(port);
//    if (inet_pton(AF_INET, host, &addr.sin_addr) < 0)
//        FATAL("Address convert failed!")
//
//    // Connect
//    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
//        FATAL("Connection failed!")
//
//    INFO("Client connected to " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port))
//}
//
//Server::~Server() {
//    INFO("Server stopping...")
//    for (int fd: client_fds)
//        close(fd);
//    close(master_fd);
//}
//
//bool Client::recvFromServer() {
//    bzero(&buffer, sizeof(buffer));
//    int bytes_cnt = recv(sock, buffer, sizeof(buffer), 0);
//    if (bytes_cnt <= 0) {
//        if (bytes_cnt != 0)
//            WARN("Server recv failed: " << bytes_cnt)
//        close(sock);
//        INFO("Server disconnected!")
//
//        return false;
//    }
//
//    HandleMessage(buffer);
//    return true;
//}
//
//void Client::handle() {
//    FD_ZERO(&active_fds);
//    FD_SET(sock, &active_fds);
//
//    INFO("Waiting for activity...")
//    int sel = select(sock + 1, &active_fds, nullptr, nullptr, nullptr);
//    if (sel < 0 && errno != EINTR)
//        WARN("Select failed!")
//    else {
//        if (!recvFromServer()) {
//            // Disconnected
//
//        }
//    }
//}
//
//void Client::sendToServer(char *msg) const {
//    if (is_master)
//        game.server->HandleMessage(master_fd, msg);
//    else
//        send(sock, msg, strlen(msg), 0);
//}
//
//void Client::SendRegister(std::string &name) {
//    bzero(message, sizeof(message));
//    message[0] = char(HiyaOperation::REGISTER);
//    message[1] = char(name.length());
//    strncpy(message + 2, name.c_str(), name.length());
//    sendToServer(message);
//}
//
//void Client::SendAttack(int target, int lines) {
//    INFO("Sending attack to " << target << " with " << lines << "lines")
//    bzero(message, sizeof(message));
//    message[0] = char(HiyaOperation::ATTACK);
//    message[1] = char(target);
//    message[2] = char(lines);
//    sendToServer(message);
//}
//
//void Client::SendDead() {
//    INFO("Sending player death message")
//    bzero(message, sizeof(message));
//    message[0] = char(HiyaOperation::DEATH);
//    sendToServer(message);
//}
//
//void Client::SendUpdateBoard(char *board_encoding) {
////    INFO("Sending updated board")
////    bzero(message, sizeof(message));
////    message[0] = char(HiyaOperation::UPDATE_BOARD);
////    strncpy(message + 1, board_encoding, TILE_COUNT_H * TILE_COUNT_V);
////    sendToServer(fd, message);
//}
//
//void Client::HandleMessage(char *msg) {
////    INFO("Handling operation")
////    const auto op = HiyaOperation(msg[0]);
////
////    if (op == HiyaOperation::REGISTER) {
////        id = int(msg[1]);
////        INFO("Registed with id " << id)
////    } else if (op == HiyaOperation::UPDATE_BOARD) {
////        SendUpdateBoard(fd, msg + 1);
////    } else if (op == HiyaOperation::ATTACK) {
////        SendAttack(fd, int(msg[1]), int(msg[2]));
////    } else if (op == HiyaOperation::DEATH) {
////        SendPlayerDead(fd);
////    } else {
////        WARN("Operation " << int(op) << " not recognized")
////    }
//}
//
