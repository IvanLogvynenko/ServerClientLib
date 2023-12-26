#pragma once
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "debugKit/basic.hpp"

#include "Connection/Connection.hpp"

#include "Exceptions/SocketOpeningException.hpp"
#include "Exceptions/ConnectionException.hpp"
#include "Exceptions/DisconnectionException.hpp"

class Client
{
protected:
    u_int16_t m_socket_fd;
    Connection m_connection;
    bool m_connection_established;
    Connection m_connectTo(const char*, const u_int16_t);
public:
    Client();
    ~Client();
    Connection connectTo(const char*, const char*);
    Connection connectTo(const char*, const std::string);
    Connection connectTo(const char*, const int);

    void disconnect();

    operator int();
};
