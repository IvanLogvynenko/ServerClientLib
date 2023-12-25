#pragma once
#include "ServerException.hpp"

class HostException : ServerException
{
public:
    HostException(u_int16_t);
    ~HostException();
    const std::string what() override;
};
