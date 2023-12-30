#pragma once
#include <string>
#include <vector>
#include <functional>

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
#include "Exceptions/TimeOutException.hpp"

#define LISTEN_BACKLOG 20

class Server
{
protected:
    std::vector<Connection*> m_connections;
    u_int16_t m_port;
    int m_socket_fd;
    std::function<void(Connection&)> m_on_connect;
public:
    Server();
    ~Server();

    void host(const std::string port);
    void host(const char* port);
    void host(const int port);

    Connection& allowConnection();
    Connection& awaitNewConnection();
    Connection& awaitNewConnection(int);

    Connection& allowConnection(std::function<void(Connection&)>);
    Connection& awaitNewConnection(std::function<void(Connection&)>);
    Connection& awaitNewConnection(int, std::function<void(Connection&)>);

    u_int16_t getPort();
    Connection** getConnections();

    operator int();
    operator u_int16_t();
    operator pollfd();
    operator pollfd*();

    Server& operator=(const Server& other);

private:
    void host(const u_int16_t);
};
