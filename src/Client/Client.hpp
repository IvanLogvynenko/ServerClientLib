#pragma once

#include "debugKit/basic.hpp"

#include "Connection/Connection.hpp"

class Client
{
private:
    
public:
    Client();
    ~Client();
    Connection connect();
};
