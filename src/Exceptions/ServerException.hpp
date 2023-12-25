#pragma once
#include <string>
#include <exception>

class ServerException : public std::exception
{
protected:
    u_int16_t m_port;
    u_int16_t m_socket_fd;
public:
    ServerException(u_int16_t, u_int16_t);
    virtual const std::string what();
    ~ServerException();
};
