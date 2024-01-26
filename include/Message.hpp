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
    virtual ~Message();

    virtual size_t size();

    virtual operator std::string();
    virtual operator const char*();
};
