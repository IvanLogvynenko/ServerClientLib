#include "ConnectionNotEstablishedException.hpp"

ConnectionNotEstablishedException::ConnectionNotEstablishedException() {}

ConnectionNotEstablishedException::~ConnectionNotEstablishedException() {}

const std::string ConnectionNotEstablishedException::what()
{
    return "Connection is not established";
}
