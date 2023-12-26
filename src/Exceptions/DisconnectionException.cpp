#include "DisconnectionException.hpp"

DisconnectionException::DisconnectionException(u_int16_t socket_fd) : m_socket_fd(socket_fd){}
DisconnectionException::~DisconnectionException() {}

const std::string DisconnectionException::what() {
    std::string output = "Failed to disconnect on socket ";
    output.append(std::to_string(this->m_socket_fd));
    return output;
}
