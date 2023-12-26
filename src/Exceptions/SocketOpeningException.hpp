#pragma once
#include "ClientException.hpp"

class SocketOpeningException : public ClientException
{
public:
    SocketOpeningException();
    ~SocketOpeningException();
    const std::string what();
};
