#pragma once

#include <string>
#include <unistd.h>
#include <poll.h>

class Connection
{
private:
    std::string m_remote_name;
    int m_socket_fd;
public:
    Connection(int socket_fd);
    Connection(std::string, int);
    
    ~Connection();

    operator int() const;
    operator std::string() const;
    operator pollfd() const;
};


