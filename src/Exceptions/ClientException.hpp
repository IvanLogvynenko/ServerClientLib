#pragma once
#include "Exception.hpp"

class ClientException : public Exception
{
public:
    ClientException();
    ~ClientException();
    const std::string what();
};
