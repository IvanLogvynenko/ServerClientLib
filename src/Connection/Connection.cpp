#include "Connection.hpp"

u_int16_t Connection::id = 0;

Connection::Connection(u_int16_t ID, u_int16_t socket_fd, u_int16_t port) :
    ID(ID),
    m_socket_fd(socket_fd),
    m_port(port) {}

Connection::Connection(u_int16_t socket_fd, u_int16_t port) : 
    Connection((socket_fd != (u_int16_t)-1 && port != (u_int16_t)-1 ? id++ : (u_int16_t)-1), socket_fd, port) {}

Connection::Connection(int fd, int port) : 
    Connection((u_int16_t)fd, (u_int16_t)port) {}

Connection::~Connection() {
    close(this->m_socket_fd);
}

bool Connection::checkValidity()
{
    return this->m_socket_fd != (u_int16_t)-1 && this->m_port != (u_int16_t)-1;
}

u_int16_t Connection::getID() {
    return this->ID;
}

Connection::operator int() const {
    return this->m_socket_fd;
}
Connection::operator pollfd() const {
    return pollfd{this->m_socket_fd, POLLIN, 0};
}

Connection& Connection::operator=(const Connection& instance)
{
    if (this != &instance) {
        ID = instance.ID;
        m_socket_fd = instance.m_socket_fd;
        m_port = instance.m_port;
    }
    return *this;
}
