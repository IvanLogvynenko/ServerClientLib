#pragma once
#include "../Exceptions/Exception.hpp"

class TimeOutException : public Exception
{
public:
    TimeOutException();
    ~TimeOutException();
    const std::string what();
};
