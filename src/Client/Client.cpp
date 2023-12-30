#include "Client.hpp"

Client::Client() : m_socket_fd(-1), m_connection(Connection(-1, -1)) {
        LOG("Created client");
}
Client::~Client() {
    ILOG("Closing socket");
    close(*this);
}

Connection& Client::connectTo(const char* host, const char * port) {
    return this->m_connectTo(host, port);
}
Connection& Client::connectTo(const char* host, const std::string port) {
    return this->m_connectTo(host, port.c_str());
}

Connection& Client::m_connectTo(const char* host, const char* port) {
    // struct addrinfo hints;
    // struct addrinfo* res;
    // hints.ai_family = AF_INET;
    // hints.ai_socktype = SOCK_STREAM;

    // LOG("Getting information about host");
    // if (int status = getaddrinfo(host, DEFAULT_PORT, &hints, &res); status != 0) {
    //     freeaddrinfo(res);
    //     throw std::runtime_error("Getaddrinfo error: " + std::string(gai_strerror(status)));
    // }
    // LOG("Got information. Opening socket");

    // this->m_socket_fd = this->openSocket(res);

    // LOG("Current socket_fd is " << this->m_socket_fd);
    // LOG("Current data in `errno`: " << strerror(errno));
    // LOG("Attempting to connect to " << host);

    // bool connected = !connect(this->m_socket_fd, res->ai_addr, res->ai_addrlen);
    // for (size_t i = 1; i <= MAX_CONNECTION_ATTEMPTS && !connected; i++)
    // {
    //     LOG("Connection failed. Reconnecting... Attempt" << i);
    //     connected = !connect(this->m_socket_fd, res->ai_addr, res->ai_addrlen);
    // }
    // if (connected) {
    //     LOG("Connected successfully. Saving connection data...");
    // }
    // else {
    //     EL("Failed to connect. Amount of attempts exceeded");
    //     throw ConnectionException(host, port);
    // }
    
    // this->m_connection = Connection(*this, atoi(port));
    // LOG("Data saved");
    // freeaddrinfo(res);
    // return this->m_connection;

    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;;


    if (int status = getaddrinfo(host, port, &hints, &res); status != 0) {
        freeaddrinfo(res);
        throw std::runtime_error("Getaddrinfo error: " + std::string(gai_strerror(status)));
    }

    int _socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (_socket == -1) {
        freeaddrinfo(res);
        throw std::runtime_error("Socket error " + std::string(strerror(errno)));
    }

    int attempt = 0;
    while (attempt++ < MAX_CONNECTION_ATTEMPTS && connect(_socket, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "Worker: failed attempt " << attempt << std::endl;
        close(_socket);
    }

    freeaddrinfo(res);

    if (_socket == -1)
        throw std::runtime_error("Impossible to connect to server");

    LOG("Socket fd: " << _socket);
    return this->m_connection;
}

u_int16_t Client::openSocket(struct addrinfo* res)
{
    if (this->m_socket_fd != (u_int16_t)-1) {
        EL("You are already connected. Disconnect first");
        throw NotDisconnectedException();
    }
    // LOG(res->ai_family);
    // LOG(AF_INET);
    // LOG(res->ai_socktype);
    // LOG(SOCK_STREAM);
    // LOG(res->ai_protocol);
    // LOG(IPPROTO_TCP);
    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    LOG("Socket opened");
    if (socket_fd == -1) {
        EL("Socket opening failed. Error: " << strerror(errno));
        throw SocketOpeningException();
    }
    LOG("Socket is valid");
    return (u_int16_t)socket_fd;
}

void Client::disconnect() {
    LOG("Closed socket");
    close(this->m_socket_fd);
    this->m_socket_fd = -1;
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
    LOG("Socket fd: " << this->m_socket_fd);
    LOG("Message text: " << *message);
    LOG("Message length: " << message->size());
    if (send(this->m_socket_fd, "Hello", 5, 0) == -1) {
        EL("Message was not sent. Error: " << strerror(errno));
        throw MessageSendingException();
    } 
    LOG("Message sent");
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
