#include "Message.hpp"

Message::Message() : Message(""){}

Message::Message(std::string data) : m_data(data) {}
Message::~Message() {}

Message::operator std::string() {
    return this->m_data;
}
