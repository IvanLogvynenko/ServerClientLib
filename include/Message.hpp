#pragma once
#include <string>

#ifndef DEFAULT_MESSAGE
    #define DEFAULT_MESSAGE ""
#endif // !DEFAULT_MESSAGE

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
