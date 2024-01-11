#pragma once
#include <string>
#include <string.h>
#include <memory>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "basic.hpp"

#include "Connection.hpp"

#include "Message.hpp"
#include "Responce.hpp"

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 100
#endif // !BUFFER_SIZE

class Client
{
protected:
    int m_socket_fd;
    Connection* m_connection;

    static int openSocket(struct addrinfo* res);
public:
    Client();
    ~Client();

    Connection& connectTo(const char* = "127.0.0.1", const char* = DEFAULT_PORT);

    void disconnect();
    bool checkIfConnected()const ;

    void sendMessage(const char* = DEFAULT_MESSAGE) const;
    void sendMessage(std::string = DEFAULT_MESSAGE) const;
    void sendMessage(Message&) const;

    std::unique_ptr<Responce> recieveMessage() const;

    operator int() const;
    Client& operator=(const Client& other);
};
