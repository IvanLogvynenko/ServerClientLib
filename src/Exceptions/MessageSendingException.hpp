#include "../Exceptions/ClientException.hpp"

class MessageSendingException : public ClientException
{
public:
    MessageSendingException();
    ~MessageSendingException();
    const std::string what();
};
