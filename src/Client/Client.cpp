#include "Client.hpp"

Client::Client() : m_connection(Connection("", -1)), m_connection_established(false) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    LOG("Socket opened");
    if (socket_fd == -1) {
        EL("Socket opening failed");
        throw SocketOpeningException();
    }
    LOG("Socket is valid");
    this->m_socket_fd = (u_int16_t)socket_fd;
}

Client::~Client() {
    ILOG("Closing socket");
    close(*this);
}

Connection Client::connectTo(const char* host, const char * port) {
    return this->connectTo(host, atoi(port));
}
Connection Client::connectTo(const char* host, const std::string port) {
    return this->connectTo(host, stoi(port));
}
Connection Client::connectTo(const char* host, const int port) {
    return this->m_connectTo(host, (u_int16_t)port);
}

Connection Client::m_connectTo(const char* host, const u_int16_t port) {
    struct sockaddr_in socketAddr;
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_port = htons(port);
    socketAddr.sin_addr.s_addr = inet_addr(host);
    LOG("Attempting to connect to " << host);
    if (connect(*this, (struct sockaddr*)&socketAddr, sizeof socketAddr)) {
        EL("Connection failed");
        throw ConnectionException(host, std::to_string(port).c_str());
    }
    LOG("Connected successfully");
    this->m_connection_established = true;
    return Connection(*this);
}

void Client::disconnect() {
    if (shutdown(*this, 2)) {
        EL("Failed to disconnect");
        throw DisconnectionException(this->m_socket_fd);
    }
    this->m_connection = Connection("", -1);
    this->m_connection_established = false;
}

Client::operator int() {
    return this->m_socket_fd;
}
