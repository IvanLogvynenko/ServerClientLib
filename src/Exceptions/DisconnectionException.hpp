
#pragma once
#include "ClientException.hpp"

class DisconnectionException : public ClientException
{
protected:
    u_int16_t m_socket_fd;
public:
    DisconnectionException(u_int16_t socket_fd);
    ~DisconnectionException();
    const std::string what();
};
