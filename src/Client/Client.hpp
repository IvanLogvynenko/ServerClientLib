#pragma once
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "debugKit/basic.hpp"

#include "Connection/Connection.hpp"

#include "Message/Message.hpp"
#include "Responce/Responce.hpp"

#include "Exceptions/SocketOpeningException.hpp"
#include "Exceptions/ConnectionException.hpp"
#include "Exceptions/DisconnectionException.hpp"
#include "Exceptions/MessageSendingException.hpp"

class Client
{
protected:
    u_int16_t m_socket_fd;
    Connection m_connection;

    Connection m_connectTo(const char*, const u_int16_t);
    static u_int16_t openSocket();
public:
    Client();
    ~Client();
    Connection connectTo(const char*, const char*);
    Connection connectTo(const char*, const std::string);
    Connection connectTo(const char*, const int);

    void disconnect();
    bool checkIfConnected();

    void sendMessage(std::string message);
    void sendMessage(Message* Message);

    operator int();
    operator u_int16_t();
    Client& operator=(const Client& other);
};
