#include "BindException.hpp"

BindException::BindException(u_int16_t port, u_int16_t socket_fd) : ServerException(port, socket_fd) {}

BindException::~BindException() {}

const std::string BindException::what()
{
    std::string result;
    result.append("Binding failed ");
    result.append(std::to_string(this->m_port));
    result.append(" with socket descriptor ");
    result.append(std::to_string(this->m_socket_fd));
    return result;
}