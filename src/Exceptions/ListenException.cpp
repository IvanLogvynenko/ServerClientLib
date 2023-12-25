#include "ListenException.hpp"

ListenException::ListenException(u_int16_t socket_fd) : ServerException(-1, socket_fd) {}

ListenException::~ListenException() {}

const std::string ListenException::what()
{
    std::string result;
    result.append("Failed to listen on socket ");
    result.append(std::to_string(this->m_socket_fd));
    return result;
}