#pragma once
#include <string>
#include <array>

#include "Message.hpp"

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 100
#endif // !BUFFER_SIZE

class Responce : public Message
{
public:
    Responce();
    Responce(std::string data);
    Responce(std::array<char, BUFFER_SIZE> data, size_t length);
    ~Responce();

    operator std::string();
};
