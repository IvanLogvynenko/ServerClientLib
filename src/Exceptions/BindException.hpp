#pragma once
#include "ServerException.hpp"

class BindException : public ServerException
{
private:
public:
    BindException(u_int16_t, u_int16_t);
    ~BindException();
    const std::string what() override;
};
