#pragma once

#include <string>
#include <unistd.h>
#include <poll.h>

#include "debugKit/basic.hpp"

class Connection
{
private:
    static u_int8_t id;
protected:
    Connection(u_int8_t, int, int, bool);

    bool isEmpty;
    u_int8_t ID;
    int m_socket_fd;
    int m_port;

public:
    Connection();
    Connection(int, int);
    Connection(const Connection&);
    ~Connection();

    const static Connection empty;

    bool ifEmpty() const;
    bool checkValidity() const;

    u_int8_t getID() const;

    operator int() const;
    operator pollfd() const;

    Connection& operator=(const Connection&);
};


