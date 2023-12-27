#pragma once
#include "Exception.hpp"

class ConnectionException : public Exception
{
protected:
    const char* m_host, *m_port;
public:
    ConnectionException(const char*, const char*);
    ConnectionException(const std::string, const std::string);
    ~ConnectionException();
    const std::string what();
};
