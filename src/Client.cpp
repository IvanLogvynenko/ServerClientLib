#include "Client.hpp"

Client::Client() {
    m_connection = Connection::empty;
}
Client::~Client() {
    LOG("Client closed");
}

Client &Client::operator=(const Client &other)
{
    if (this == &other) return *this;
    this->m_connection = other.m_connection;
    return *this;
}
Client::operator int() const
{
    return this->m_connection;
}

//now important functions

Connection& Client::connectTo(const char* host, const char* port) {
    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int socket_fd = 0;
    
    LOG("Attempting to connect to " << host << ":" << port);
    LOG("Getting information");

    if (int status = getaddrinfo(host, port, &hints, &res); status != 0) {
        freeaddrinfo(res);
        throw std::runtime_error("Getaddrinfo error: " + std::string(gai_strerror(status)));
    }
    LOG("Got information. Opening socket...");
    socket_fd = openSocket(res);
    LOG("Socket opened. Processing to connection...");
    int attempt = 0;
    while (attempt++ < MAX_CONNECTION_ATTEMPTS && connect(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "Connection failed | attempt " << attempt << std::endl;
        close(socket_fd);
        socket_fd = -1;
    }
    if (socket_fd == -1) {
        freeaddrinfo(res);
        throw std::runtime_error("Impossible to connect to server");
    }
    LOG("Connected successfully")

    LOG("Configuring connection...");

    Connection connection = Connection(std::move(socket_fd), atoi(port));
    m_connection = std::move(connection);
    freeaddrinfo(res);
    return m_connection;
}

int Client::openSocket(struct addrinfo* res)
{   
    LOG("Attempting to open socket");
    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd == -1) {
        freeaddrinfo(res);
        throw std::runtime_error("Socket error " + std::string(strerror(errno)));
    }
    LOG("Openned successfully");
    LOG("Socket fd is " << socket_fd);
    return socket_fd;
}

void Client::disconnect()
{
    LOG("Disconnecting..");
    this->m_connection = Connection();
    LOG("Disconnected");
}

bool Client::checkIfConnected() const 
{
    return this->m_connection.checkValidity();
}

void Client::sendMessage(const char* data) const
{
    this->sendMessage((std::string)data);
}
void Client::sendMessage(std::string data) const
{
    m_connection << data;
}

std::string Client::recieveMessage() const
{
    return this->m_connection.recieve();
}
