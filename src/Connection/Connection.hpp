#pragma once

#include <string>
#include <unistd.h>
#include <poll.h>

#include "debugKit/basic.hpp"

class Connection
{
private:
    static unsigned short id;
protected:
    Connection(u_int16_t, u_int16_t, u_int16_t);

    unsigned short ID;
    u_int16_t m_socket_fd;
    u_int16_t m_port;

public:
    Connection(u_int16_t, u_int16_t);
    Connection(int, int);
    ~Connection();

    bool checkValidity();

    unsigned short getID();

    operator int() const;
    operator pollfd() const;

    Connection& operator=(const Connection& instance);
};


