#pragma once
#include <string>

#include "../Message/Message.hpp"

class Responce : public Message
{
public:
    Responce();
    Responce(std::string data);
    ~Responce();

    operator std::string();
};
