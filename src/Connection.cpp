#include "Connection.hpp"

using namespace server_client;

unsigned int Connection::ID = 0;
const std::string Connection::CLOSE_MESSAGE = "SYSTEM_MESSAGE<CONNECTION CLOSED>";
const std::string Connection::SERVER_STOP = "SYSTEM_MESSAGE<SERVER STOP>";

Connection::Connection(int sd) : 
	sd(sd), 
	id(ID++),
	backlog("") {}
Connection::Connection(const Connection& other) : 
    sd(other.sd), 
    id(other.id),
	backlog(other.backlog) {}
Connection& Connection::operator=(const Connection& other) {
	if (&other == this) 
		return *this;

	this->id = other.id;
	this->sd = other.sd;
	this->backlog = other.backlog;
	return *this;
}



server_client::Connection::~Connection() {
	close(this->sd);
}
