#include "Server.hpp"

Server::Server() : 
    m_connections({}),
    m_port((u_int16_t)-1), 
    m_socket_fd(-1),
    m_on_connect(nullptr) {}

Server::~Server() {
    LOG("Closing socket");
    close(*this);
    for (size_t i = 0; i < this->m_connections.size(); i++)
    {
        delete this->m_connections[i];
    }
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

    if (bind(this->m_socket_fd, (struct sockaddr*)&socketAddr, sizeof(socketAddr)) == -1) {
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

/// @brief use it only if you're sure that there is a connection awaiting for you to capture it, 
/// otherwise awaitNewConnection() is a better option
/// @return Returns an instance of Connection with socket descriptor and port recieved during connection
Connection& Server::allowConnection()
{
    return this->allowConnection(nullptr);
}
//https://stackoverflow.com/questions/529975/what-does-poll-do-with-a-timeout-of-0 so it should be -1 and not 0
Connection& Server::awaitNewConnection()
{
    return this->awaitNewConnection(-1);
}
Connection& Server::awaitNewConnection(int awaitMilliseconds)
{
    return this->awaitNewConnection(awaitMilliseconds, nullptr);
}
Connection& Server::allowConnection(std::function<void(Connection&)> on_connect)
{
    sockaddr_in incomming;
    socklen_t length = sizeof(struct sockaddr_in);
    LOG("About to accept new connection");
    int new_connection_fd = accept(*this, (struct sockaddr *)&incomming, &length);
    if (new_connection_fd == -1) {
        EL("Failed to create a connection");
        throw ConnectionException(std::to_string(this->m_socket_fd), std::to_string(incomming.sin_port));
    }
    LOG("Captured socket fd! Value: " << new_connection_fd);
    ILOG("Connection accepted!");
    Connection* connection = new Connection((u_int16_t)new_connection_fd, incomming.sin_port);
    LOG("Configuring new connection");
    if (on_connect != nullptr) {
        on_connect(*connection);
        this->m_on_connect = on_connect;
    }
    else if (this->m_on_connect != nullptr)
        this->m_on_connect(*connection);
    LOG("About to finish configuring");
    this->m_connections.push_back(connection);
    LOG("Configuring finished. Ready to use");
    return *connection;
}
Connection &Server::awaitNewConnection(std::function<void(Connection&)> on_connect)
{
    return this->awaitNewConnection(-1, on_connect);
}

Connection &Server::awaitNewConnection(int awaitMilliseconds, std::function<void(Connection&)> on_connect)
{
    if (!awaitMilliseconds)
        return this->allowConnection(on_connect);
    LOG("Awaiting new connection");
    
    pollfd pfd = *this;
    if (!poll(&pfd, 1, awaitMilliseconds)) {
        EL("Havent recieved any connection attempt in specified time");
        throw TimeOutException();
    }
    LOG("Poll() finished. Proceeding to connection configuration");

    sockaddr_in incomming;
    socklen_t length = sizeof(struct sockaddr_in);
    int new_connection_fd = accept(*this, (struct sockaddr *)&incomming, &length);
    LOG("Got new connection");
    Connection* connection = new Connection((u_int16_t)new_connection_fd, incomming.sin_port);
    if (on_connect != nullptr) {
        on_connect(*connection);
        this->m_on_connect = on_connect;
    }
    else if (this->m_on_connect != nullptr)
        this->m_on_connect(*connection);
    this->m_connections.push_back(connection);
    LOG("Adding of a connection finished");
    return *connection;
}

u_int16_t Server::getPort() {
    return this->m_port;
}

Connection** Server::getConnections()
{
    return this->m_connections.data();
}

//operators
Server::operator int() {
    return this->m_socket_fd;
}
Server::operator u_int16_t()
{
    return (u_int16_t)this->m_socket_fd;
}
Server::operator pollfd()
{
    return pollfd{*this, POLLIN, 0};
}
Server::operator pollfd*()
{
    std::vector<pollfd> result = {};
    result.push_back(*this);
    for (size_t i = 0; i < this->m_connections.size(); i++)
        result.push_back(*(this->m_connections[i]));   
    return result.data(); 
}
Server &Server::operator=(const Server &other) {
    if (this != &other) {
        this->m_connections = other.m_connections;
        this->m_port = other.m_port;
        this->m_socket_fd = other.m_socket_fd;
    }
    return *this;
}