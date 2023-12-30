#include "ClientException.hpp"

class NotDisconnectedException : public ClientException
{
public:
    NotDisconnectedException();
    ~NotDisconnectedException();
    const std::string what();
};
