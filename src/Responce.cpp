#include "Responce.hpp"

Responce::Responce() : Responce("") {}
Responce::Responce(std::string data) : Message(data) {}
Responce::Responce(std::array<char, BUFFER_SIZE> data, size_t length) :
    Responce(std::string(data.data(), length)){}

Responce::~Responce() {}

