#include "ConnectionException.hpp"

ConnectionException::ConnectionException(const char* host, const char* port) : m_host(std::move(host)), m_port(std::move(port)) {}

ConnectionException::~ConnectionException() {}

const std::string ConnectionException::what()
{
    std::string output = "";
    output.append("Failed to establish connection with ");
    output.append(m_host);
    output.append(" on port ");
    output.append(m_port);
    return output;
}
