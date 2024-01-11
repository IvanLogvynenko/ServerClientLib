#pragma once
#include <string>

class Message
{
protected:
    std::string m_data;
public:
    Message(std::string = DEFAULT_MESSAGE);
    ~Message();

    size_t size();

    operator std::string();
    operator const char*();
};
