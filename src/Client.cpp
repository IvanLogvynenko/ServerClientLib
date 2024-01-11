#include "Client.hpp"

Client::Client() : 
    m_socket_fd(0),
    m_connection(nullptr) 
{}
Client::~Client() {
    LOG("Client closed");
    delete m_connection;
    close(m_socket_fd);
}

Client &Client::operator=(const Client &other)
{
    if (this != &other) {
        this->m_socket_fd = other.m_socket_fd;
        if (m_connection != other.m_connection) {
            delete m_connection;
            this->m_connection = other.m_connection;
        }
    }
    return *this;
}
Client::operator int() const
{
    return this->m_socket_fd;
}

//now important functions

Connection& Client::connectTo(const char* host, const char* port) {
    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    LOG("Attempting to connect to " << host << ":" << port);
    LOG("Getting information");

    if (int status = getaddrinfo(host, port, &hints, &res); status != 0) {
        freeaddrinfo(res);
        throw std::runtime_error("Getaddrinfo error: " + std::string(gai_strerror(status)));
    }
    LOG("Got information. Opening socket...");
    this->m_socket_fd = openSocket(res);
    LOG("Socket opened. Processing to connection...");
    int attempt = 0;
    while (attempt++ < MAX_CONNECTION_ATTEMPTS && connect(this->m_socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "Connection failed | attempt " << attempt << std::endl;
        close(this->m_socket_fd);
        this->m_socket_fd = -1;
    }
    if (this->m_socket_fd == -1) {
        freeaddrinfo(res);
        throw std::runtime_error("Impossible to connect to server");
    }
    LOG("Connected successfully")

    LOG("Configuring connection...");

    Connection* connection = new Connection(this->m_socket_fd, atoi(port));
    m_connection = std::move(connection);
    freeaddrinfo(res);
    return *m_connection;
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
    close(this->m_socket_fd);
    this->m_socket_fd = -1;
    if (!this->m_connection->ifEmpty())
        delete this->m_connection;
    this->m_connection = new Connection();
    LOG("Disconnected");
}

bool Client::checkIfConnected() const 
{
    return this->m_socket_fd != -1 
        && this->m_connection->checkValidity();
}

void Client::sendMessage(const char* data) const
{
    Message message = Message(data);
    this->sendMessage(message);
}
void Client::sendMessage(std::string data) const
{
    Message message = Message(data);
    this->sendMessage(message);
}

void Client::sendMessage(Message& message) const
{
    LOG("Sending message: " << message);
    LOG("Current socket fd is " << this->m_socket_fd);
    if (send(*m_connection, message, message.size(), 0) == -1) {
        EL("Message sending failed");
        throw std::runtime_error("Message sending error");
    }
}

std::unique_ptr<Responce> Client::recieveMessage() const
{
    std::array<char, BUFFER_SIZE> buffer;
    int data_size = 0;
    int socket_to_recieve_from = *(this->m_connection);
    if (data_size = (int)recv(socket_to_recieve_from, buffer.data(), BUFFER_SIZE, 0); data_size == -1) {
        EL("Failed to recieve data from " << socket_to_recieve_from);
        throw std::runtime_error("Failed to recieve data");
    }
    std::unique_ptr<Responce> responce = std::make_unique<Responce>(buffer, data_size);
    return responce;
}
