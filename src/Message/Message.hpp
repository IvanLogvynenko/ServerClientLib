#pragma once
#include <string>

class Message
{
protected:
    std::string m_data;
public:
    Message();
    Message(std::string data);
    ~Message();

    operator std::string();
};
