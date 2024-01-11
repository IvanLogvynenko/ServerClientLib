#include "Connection.hpp"

u_int8_t Connection::id = 1;
const Connection Connection::empty = Connection();

/// @brief Basic constructor
/// @param socket_fd 
/// @param port 
Connection::Connection(int socket_fd, int port) :
    m_socket_fd(socket_fd),
    m_port(port)
{
    isEmpty = socket_fd == -1 || port == -1;
    if (isEmpty) 
        ID = 0;
    else {
        ID = id++; 
        LOG("Connection " << (int)this->ID << " opened");
    }
    
}

Connection::~Connection() {
    if (this->ID != 0)
        LOG("Connection " << (int)this->ID << " closed");
}

Connection & Connection::operator=(const Connection& other)
{
    if (this != &other) {
        this->ID = other.ID;
        this->m_socket_fd = other.m_socket_fd;
        this->m_port = other.m_port;
        this->isEmpty = other.isEmpty;
    }
    return *this;
}

bool Connection::ifEmpty() const
{
    return this->isEmpty;
}

// now important functions
bool Connection::checkValidity() const
{
    return this->m_socket_fd != -1 
        && this->m_port != -1 
        && !this->isEmpty;
}
u_int8_t Connection::getID() const
{
    return this->ID;
}
Connection::operator int() const
{
    return this->m_socket_fd;
}
Connection::operator pollfd() const
{
    return (pollfd) {
        this->m_socket_fd, 
        POLLIN, 
        0
    };
}

