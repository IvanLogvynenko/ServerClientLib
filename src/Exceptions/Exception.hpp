#pragma once
#include <string>

class Exception
{
public:
    Exception();
    ~Exception();
    virtual const std::string what();
};
