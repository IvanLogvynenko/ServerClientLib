#include "Connection.hpp"

u_int8_t Connection::id = 1;
const Connection Connection::empty = Connection();

/// @brief This is a constructor used by copying operator
/// @param ID Connection ID
/// @param socket_fd Socket descriptor
/// @param port port to connect
/// @param isEmpty if this connection is empty it would be true
Connection::Connection(u_int8_t ID, int socket_fd, int port, bool isEmpty)
{
    if (socket_fd == -1 || port == -1) 
        Connection();
    this->ID = ID;
    this->m_socket_fd = socket_fd;
    this->m_port = port;
    this->isEmpty = isEmpty;
}
/// @brief Creates an empty connection so feel free to use
Connection::Connection() : 
    isEmpty(true),
    ID(0),
    m_socket_fd(-1),
    m_port(-1)
{}
/// @brief Basic constructor
/// @param socket_fd 
/// @param port 
Connection::Connection(int socket_fd, int port) :
    isEmpty(false),
    m_socket_fd(socket_fd),
    m_port(port)
{
    if (socket_fd == -1 || port == -1) 
        ID = 0;
    else {
        ID = id++; 
        LOG("Connection " << this->ID << " opened");
    }
    
}
/// @brief coping onstructor
/// @param other instance to cpoy from
Connection::Connection(const Connection &other) :
    Connection(
        other.ID, 
        other.m_socket_fd, 
        other.m_port,
        other.isEmpty) 
    {}

Connection::~Connection() {
    LOG("Connection " << this->ID << " closed");
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

//now important functions
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

