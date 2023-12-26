#pragma once
#include "ClientException.hpp"

class ConnectionNotEstablishedException : public ClientException
{
public:
    ConnectionNotEstablishedException();
    ~ConnectionNotEstablishedException();
};
