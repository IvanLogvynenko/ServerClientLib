#pragma once
#include <string>

#include <vector>
#include <array>
#include <unordered_map>
#include <queue>

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

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 100
#endif // !BUFFER_SIZE

#ifndef DEFAULT_PORT
    #define DEFAULT_PORT "37373"
#endif // !DEFAULT_PORT

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
    
    std::atomic_bool m_server_destructing_allowed;
public:
    Server(
        std::function<void(Connection&)> = nullptr, 
        int = -1, 
        std::string = std::string()
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

    operator int();

    Connection& operator[](size_t);

    size_t getAmountOfConnections();

    Server& operator=(const Server& other);
};