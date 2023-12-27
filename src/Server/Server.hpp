#pragma once
#include <string>
#include <vector>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

#include "debugKit/basic.hpp"

#include "Connection/Connection.hpp"

#include "Exceptions/HostException.hpp"
#include "Exceptions/BindException.hpp"
#include "Exceptions/ListenException.hpp"
#include "Exceptions/ConnectionException.hpp"

#define LISTEN_BACKLOG 20

class Server
{
protected:
    std::vector<Connection*> m_connections;
    u_int16_t m_port;
    int m_socket_fd;
    void(*m_onConnect)(Connection&);
public:
    Server();
    ~Server();

    void host(const std::string port);
    void host(const char* port);
    void host(const int port);

    Connection& allowConnection();
    Connection& awaitNewConnection();
    Connection& awaitNewConnection(int);

    Connection& allowConnection(void(*)(Connection&));
    Connection& awaitNewConnection(void(*)(Connection&));
    Connection& awaitNewConnection(int, void(*)(Connection&));

    u_int16_t getPort();

    operator int();
    operator u_int16_t();
    operator pollfd();
    operator std::vector<pollfd>();

    Server& operator=(const Server& other);

private:
    void host(const u_int16_t);
};
