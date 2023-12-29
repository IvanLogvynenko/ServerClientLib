#include "Message.hpp"

Message::Message() : Message(""){}

Message::Message(std::string data) : m_data(data) {}
Message::~Message() {}

size_t Message::size()
{
    return this->m_data.length();
}

Message::operator std::string() {
    return this->m_data;
}

Message::operator const char *()
{
    return this->m_data.c_str();
}
