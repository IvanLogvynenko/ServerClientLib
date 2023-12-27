#pragma once
#include "Exceptions/Exception.hpp"

class ServerException : public Exception
{
protected:
    u_int16_t m_port;
    u_int16_t m_socket_fd;
public:
    ServerException(u_int16_t, u_int16_t);
    const std::string what();
    ~ServerException();
};
