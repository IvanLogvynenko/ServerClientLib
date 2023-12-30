#pragma once
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "debugKit/basic.hpp"

#include "Connection/Connection.hpp"

#include "Message/Message.hpp"
#include "Responce/Responce.hpp"

#include "Exceptions/SocketOpeningException.hpp"
#include "Exceptions/ConnectionException.hpp"
#include "Exceptions/DisconnectionException.hpp"
#include "Exceptions/MessageSendingException.hpp"
#include "Exceptions/NotDisconnectedException.hpp"

#include <string.h>
#include <memory>


class Client
{
protected:
    u_int16_t m_socket_fd;
    Connection m_connection;

    Connection& m_connectTo(const char*, const char*);
    u_int16_t openSocket(struct addrinfo* res);
public:
    Client();
    ~Client();
    Connection& connectTo(const char*, const char*);
    Connection& connectTo(const char*, const std::string);

    void disconnect();
    bool checkIfConnected();

    void sendMessage(std::string message);
    void sendMessage(Message* Message);

    operator int();
    operator u_int16_t();
    Client& operator=(const Client& other);
};
