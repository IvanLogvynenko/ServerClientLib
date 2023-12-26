#include "SocketOpeningException.hpp"

SocketOpeningException::SocketOpeningException() {}
SocketOpeningException::~SocketOpeningException() {}

const std::string SocketOpeningException::what() {
    return "Socket opening failed";
}
