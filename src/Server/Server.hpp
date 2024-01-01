#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <array>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

#include "debugKit/basic.hpp"

#include "Connection/Connection.hpp"

#include "Message/Message.hpp"
#include "Responce/Responce.hpp"

#ifndef LISTEN_BACKLOG
    #define LISTEN_BACKLOG 20
#endif // !LISTEN_BACKLOG

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 100
#endif // !BUFFER_SIZE

class Server
{
protected:
    int m_port;
    int m_socket_fd;
    std::vector<std::shared_ptr<Connection>> m_connections;
    std::function<void(Connection&)> m_on_connect;
public:
    Server();
    ~Server();

    void host(const char* = DEFAULT_PORT);

    Connection& 
    awaitNewConnection(int = DEFAULT_TIMEOUT, std::function<void(Connection&)> = nullptr);

    int getPort();
    
    std::vector<std::shared_ptr<Connection>> 
    getConnections();

    std::unique_ptr<Responce> 
    recieveMessageFrom(const int = 0) const;

    operator int();
    Server& operator=(const Server& other);
};
