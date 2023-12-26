#include "Server.hpp"

Server::Server() : m_port(-1) {}
Server::~Server() {
    LOG("Closing socket");
    close(this->m_socket_fd);
}


void Server::host(const std::string port) {
    this->host(stoi(port));
}
void Server::host(const char *port) {
    this->host(atoi(port));
}
void Server::host(const int port) {
    this->host((u_int16_t)port);
}

void Server::host(const u_int16_t port)
{
    this->m_port = port;

    LOG("Opening socket");
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == -1) {
        EL("Socket opening failed");
        throw HostException(port);
    }
    LOG("Socket opened");
    this->m_socket_fd = socket_fd;
    LOG("Binding socket");
    struct sockaddr_in socketAddr;
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_port = htons(port);
    socketAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(*this, (struct sockaddr*)&socketAddr, sizeof(socketAddr)) == -1) {
        EL("Socket binding failed");
        throw BindException(port, (u_int16_t)socket_fd);
    }

    LOG("Setting socket to listening state");
    if (listen(*this, LISTEN_BACKLOG) == -1) {
        EL("Setting socket to listening state failed");
        throw ListenException((u_int16_t)socket_fd);
    }
    ILOG("Opened and configured socket succesfully");
}

Connection Server::allowConnection()
{
    int new_connection_fd = accept(*this, nullptr, nullptr);
    Connection connection = Connection(new_connection_fd);
    this->m_connections.push_back(connection);
    return connection;
}

u_int16_t Server::getPort() {
    return this->m_port;
}

//operators
Server::operator int() {
    return this->m_socket_fd;
}