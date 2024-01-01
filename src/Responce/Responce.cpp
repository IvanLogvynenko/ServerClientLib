#include "Responce.hpp"

Responce::Responce() : Responce("") {}
Responce::Responce(std::string data) : Message(data) {}
Responce::Responce(std::array<char, BUFFER_SIZE> data, int length) :
    Responce(std::string(data.data(), length)){}

Responce::~Responce() {}

