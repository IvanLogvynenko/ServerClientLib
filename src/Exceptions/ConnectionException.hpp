#pragma once
#include "ClientException.hpp"

class ConnectionException : public ClientException
{
protected:
    const char* m_host, *m_port;
public:
    ConnectionException(const char*, const char*);
    ~ConnectionException();
    const std::string what();
};
