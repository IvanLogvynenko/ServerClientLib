#include "NotDisconnectedException.hpp"

NotDisconnectedException::NotDisconnectedException() {}

NotDisconnectedException::~NotDisconnectedException() {}

const std::string NotDisconnectedException::what()
{
    return "Connection is not closed";
}