#include "Client.hpp"

Client::Client() : 
    m_socket_fd(openSocket()), 
    m_connection(Connection(-1, -1))
    {
        LOG("Created client");
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
    return Connection(*this, port);
}

u_int16_t Client::openSocket()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    LOG("Socket opened");
    if (socket_fd == -1) {
        EL("Socket opening failed");
        throw SocketOpeningException();
    }
    LOG("Socket is valid");
    return std::move((u_int16_t)socket_fd);
}

void Client::disconnect() {
    LOG("Closed socket");
    close(this->m_socket_fd);
    this->m_socket_fd = openSocket();
    this->m_connection = Connection(-1, -1);
}
bool Client::checkIfConnected() {
    return this->m_connection.checkValidity();
}

void Client::sendMessage(std::string message) {
    Message *new_message = new Message(message);
    this->sendMessage(new_message);
    delete new_message;
}

void Client::sendMessage(Message* message) {
    LOG("About to send message, value: " << *message);
    if (send(this->m_socket_fd, *message, message->size(), 0) == -1) {
        EL("Message was not sent");
        throw MessageSendingException();
    } 
}

Client::operator int() {
    return this->m_socket_fd;
}
Client::operator u_int16_t()
{
    return this->m_socket_fd;
}
Client &Client::operator=(const Client &other)
{
    if (this != &other) {
        this->m_connection = other.m_connection;
        this->m_socket_fd = other.m_socket_fd;
    }
    return *this;
}
