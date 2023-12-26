#include "Connection.hpp"

Connection::Connection(u_int16_t socket_fd) : m_remote_name("Unnamed"), m_socket_fd(socket_fd) {}
Connection::Connection(int fd) : Connection((u_int16_t)fd) {}
Connection::Connection(std::string remote_name, u_int16_t socket_fd) : m_remote_name(remote_name), m_socket_fd(socket_fd) {}
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

