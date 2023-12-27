#include "TimeOutException.hpp"

TimeOutException::TimeOutException() {}

TimeOutException::~TimeOutException(){}

const std::string TimeOutException::what()
{
    return "Timeout in poll()";
}