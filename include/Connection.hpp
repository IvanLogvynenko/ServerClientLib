#pragma once

#include <cstdint>

#include <array>

#include <string>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "basic.hpp"

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 100
#endif // !BUFFER_SIZE

class Connection
{
private:
    static uint8_t id;
protected:
    uint8_t ID;
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

    std::string recieve() const;

    const Connection& operator<<(std::string&) const;
    const Connection& operator>>(std::string&) const;

    Connection& operator=(const Connection&);
};
