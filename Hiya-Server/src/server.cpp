#include "server.h"
#include "log.h"

Server::Server(int port) {

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
    INFO("Server destroying...")
    close(master_fd);
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

        sprintf(message, "Welcome to Hiya Server!\nEverything you typed here will be sent to all clients!\nClient count: %lu\n", client_fds.size());
        sendTo(client_fd, message);

        sprintf(message, "New client: %d!\n", client_fd);
        sendToAllOther(client_fd, message);
    }
}

bool Server::recvFromClient(int fd) {
    bzero(&buffer, sizeof(buffer));
    int bytes_cnt = recv(fd, buffer, sizeof(buffer), 0);
    if (bytes_cnt <= 0) {
        if (bytes_cnt != 0)
            WARN("Client " << fd << " recv failed: " << bytes_cnt)
        close(fd);
        FD_CLR(fd, &fds);
        INFO("Client " << fd << " disconnected!")
        return false;
    }

    INFO("Receive from client " << fd << " :" << buffer)

    sprintf(message, "User %d: %s", fd, buffer);
    sendToAllOther(fd, message);

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
                    continue;
                }
            }
            it++;
        }
    }
}

void Server::sendTo(int fd, char *message) {
    send(fd, message, strlen(message), 0);
}

void Server::sendToAllOther(int source_fd, char *message) {
    for (int fd: client_fds) {
        if (fd != source_fd)
            sendTo(fd, message);
    }
}