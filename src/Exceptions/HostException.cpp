#include "HostException.hpp"

HostException::HostException(u_int16_t port) : ServerException(port, -1) {}

HostException::~HostException() {}

const std::string HostException::what() {
    std::string result;
    result.append("Failed to host server on port ");
    result.append(std::to_string(this->m_port));
    return result;
}
