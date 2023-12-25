#include "Connection.hpp"

Connection::Connection(int socket_fd) : m_socket_fd(socket_fd){}

Connection::~Connection()
{
}

Connection::operator int() const
{
    return this->m_socket_fd;
}
