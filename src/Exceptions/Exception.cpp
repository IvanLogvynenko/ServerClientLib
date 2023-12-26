#include "Exception.hpp"

Exception::Exception() {}

Exception::~Exception() {}

const std::string Exception::what()
{
    return "If you get this message then there are two cases: \n >> 1) polymorphism sucks \n >> 2) your code sucks \nI believe second is the case";
}