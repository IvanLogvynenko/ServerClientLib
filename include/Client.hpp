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

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 100
#endif // !BUFFER_SIZE

#ifndef DEFAULT_PORT
    #define DEFAULT_PORT "37373"
#endif // !DEFAULT_PORT

#ifndef MAX_CONNECTION_ATTEMPTS
    #define MAX_CONNECTION_ATTEMPTS 50
#endif // !MAX_CONNECTION_ATTEMPTS

class Client
{
private:
    static int openSocket(struct addrinfo* res);
    
protected:
    Connection m_connection;

public:
    Client();
    ~Client();

    Connection& connectTo(const char* = "127.0.0.1", int = DEFAULT_PORT);

    void disconnect();
    bool checkIfConnected() const;

    void sendMessage(const char*) const;
    void sendMessage(std::string) const;

    std::string recieveMessage() const;

    operator int() const;
    Client& operator=(const Client& other);
};
