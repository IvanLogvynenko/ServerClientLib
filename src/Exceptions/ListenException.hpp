#pragma once
#include "ServerException.hpp"

class ListenException : public ServerException
{
public:
    ListenException(u_int16_t);
    ~ListenException();
    const std::string what() override;
};