#include "Connection.hpp"

Connection::Connection(u_int16_t socket_fd, u_int16_t port) : Connection("Unnamed", socket_fd, port) {}
Connection::Connection(int fd, int port) : Connection((u_int16_t)fd, (u_int16_t)port) {}
Connection::Connection(std::string remote_name, u_int16_t socket_fd, u_int16_t port) : 
    m_remote_name(remote_name), 
    m_socket_fd(socket_fd),
    m_port(port) {}

Connection::~Connection() {
    close(this->m_socket_fd);
}

Connection::operator int() const {
    return this->m_socket_fd;
}
Connection::operator std::string() const {
    return this->m_remote_name;
}
Connection::operator pollfd() const {
    return pollfd{this->m_socket_fd, POLLIN, 0};
}

