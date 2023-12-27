#pragma once

#include <string>
#include <unistd.h>
#include <poll.h>

// TODO: Set the code for saving 'port'
class Connection
{
private:
    static unsigned char id;
protected:
    std::string m_remote_name;
    u_int16_t m_socket_fd;
    u_int16_t m_port;

public:
    Connection(u_int16_t, u_int16_t);
    Connection(int, int);
    Connection(std::string, u_int16_t, u_int16_t);
    ~Connection();

    operator int() const;
    operator std::string() const;
    operator pollfd() const;
};


