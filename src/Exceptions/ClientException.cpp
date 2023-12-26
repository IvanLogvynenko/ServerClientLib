#include "ClientException.hpp"

ClientException::ClientException() {}
ClientException::~ClientException() {}

const std::string ClientException::what() {
    return "Exception caught in Client";
}
