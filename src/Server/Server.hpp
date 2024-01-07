#pragma once
#include <string>
#include <vector>
#include <array>

#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

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
    std::vector<std::shared_ptr<Connection>> m_connections = std::vector<std::shared_ptr<Connection>>(0);
    std::mutex m_connections_mutex;
    std::atomic_bool m_connection_handling_started;
    std::atomic_bool m_server_destructing_allowed;
    std::function<void(Connection&)> m_on_connect = nullptr;

    std::function<void(Message&)> m_on_message_income = nullptr;
    
    int m_lastly_used_connection = 0;
public:
    Server();
    ~Server();

    void host(const char* = DEFAULT_PORT);

    Connection& awaitNewConnection(std::function<void(Connection&)> = nullptr);

    int getPort();
    
    std::vector<std::shared_ptr<Connection>> getConnections();

    void sendMessage(const char* = "", size_t = 0) const;
    void sendMessage(std::string = "", size_t = 0) const;
    void sendMessage(Message&, size_t = 0) const;
    void sendMessage(Message&, const Connection& connection) const;

    std::unique_ptr<Responce> recieveMessageFrom(size_t = 0);
    std::unique_ptr<Responce> recieveMessageFrom(const Connection&);

    void respond(const char* = "") const;
    void respond(std::string = "") const;
    void respond(Message&) const;

    void startConnectionHandling(std::function<void(Connection&)> = nullptr);
    void stopConnectionHandling();

    void startMessageIncomeHandling(std::function<void(Message&)> = nullptr, bool = false);
    void stopMessageIncomeHandling();

    void startSendingMessage(Message&, int = DEFAULT_TIMEOUT);

    operator int();
    Server& operator=(const Server& other);
};
