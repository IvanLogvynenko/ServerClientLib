#include "MessageSendingException.hpp"

MessageSendingException::MessageSendingException() {}

MessageSendingException::~MessageSendingException() {}

const std::string MessageSendingException::what() {
    return "Failed to send a message";
}