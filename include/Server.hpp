#pragma once
#include <string>

#include <vector>

#include <functional>
#include <memory>

#include <thread>
#include <atomic>
#include <mutex>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

#include "basic.hpp"

#include "Connection.hpp"

#ifndef LISTEN_BACKLOG
    #define LISTEN_BACKLOG 20
#endif // !LISTEN_BACKLOG

#ifndef DEFAULT_PORT
    #define DEFAULT_PORT "37373"
#endif // !DEFAULT_PORT

#ifndef HANDLING_TIMEOUT
    #define HANDLING_TIMEOUT 500
#endif // !HANDLING_TIMEOUT

class Server
{
protected:
    int m_socket_fd;
    std::string m_port;

    std::mutex m_connections_mutex;
    std::vector<std::shared_ptr<Connection>> m_connections = std::vector<std::shared_ptr<Connection>>(0);

    int m_lastly_used_connection = 0;

    std::atomic_bool m_connection_handling_started;
    std::function<void(Connection&)> m_on_connect;

    std::atomic_bool m_message_income_handling_started;
    std::function<void(std::string&, Connection&)> m_on_recieve;
    
    std::atomic_bool m_server_destructing_allowed;
public:
    Server(
        std::string = std::string(),
        std::function<void(Connection&)> = nullptr, 
        std::function<void(std::string&, Connection&)> = nullptr, 
        int = -1
    );
    Server(Server&);
    ~Server();

    int host(std::string = DEFAULT_PORT);

    Connection& awaitNewConnection(std::function<void(Connection&)> = nullptr);

    std::string getPort();
    
    std::vector<std::shared_ptr<Connection>> getConnections();

    void sendMessage(const char* = "", size_t = 0) const;
    void sendMessage(std::string = "", size_t = 0) const;
    void sendMessage(std::string, const Connection& connection) const;

    std::string recieveMessageFrom(size_t = 0);

    std::string recieveMessageFrom(const Connection&);

    void respond(const char* = "") const;
    void respond(std::string = "") const;

    void startConnectionHandling(std::function<void(Connection&)> = nullptr, bool = false);
    void stopConnectionHandling();

    void startMessageIncomeHandling(std::function<void(std::string&, Connection&)> = nullptr);
    void stopMessageIncomeHandling();

    operator int();

    Connection& operator[](size_t);

    size_t getAmountOfConnections();

    Server& operator=(const Server& other);
};