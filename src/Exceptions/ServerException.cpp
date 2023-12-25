#include "ServerException.hpp"

ServerException::ServerException(u_int16_t port, u_int16_t socket_fd) : m_port(port), m_socket_fd(socket_fd){}

ServerException::~ServerException() {}

const std::string ServerException::what() {
    std::string result;
    result.append("Server exception on port ");
    result.append(std::to_string(this->m_port));
    result.append(" with socket descriptor ");
    result.append(std::to_string(this->m_socket_fd));
    return result;
}
