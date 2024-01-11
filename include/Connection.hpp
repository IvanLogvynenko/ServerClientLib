#pragma once

#include <string>
#include <unistd.h>
#include <poll.h>

#include "basic.hpp"

class Connection
{
private:
    static u_int8_t id;
protected:
    u_int8_t ID;
    int m_socket_fd;
    int m_port;
    bool isEmpty;

public:
    Connection(int = -1, int = -1);
    ~Connection();

    const static Connection empty;

    bool ifEmpty() const;
    bool checkValidity() const;

    u_int8_t getID() const;

    operator int() const;
    operator pollfd() const;

    Connection& operator=(const Connection&);
};


