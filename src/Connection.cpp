#include "Connection.hpp"

#include "debugKit/Logger.hpp"

unsigned int Connection::ID = 0;
const std::string Connection::CLOSE_MESSAGE = "SYSTEM_MESSAGE<CONNECTION CLOSED>";
const std::string Connection::SERVER_STOP = "SYSTEM_MESSAGE<SERVER STOP>";

Connection::Connection(int sd) : 
	sd(sd), 
	id(ID++) {}


#include <poll.h>
#include <sys/socket.h>

#include <cstddef>

std::string Connection::awaitNewMessage() const {
	Logger logger("Connection::awaitNewMessage");
	pollfd poll_data = {this->sd, POLLIN, 0 };
	logger << Logger::important << "Awaiting new message" << std::endl;
	logger << Logger::debug << "Timeout: " << MESSAGE_INCOME_TIMEOUT << "ms" << std::endl;
	int poll_res = poll(&poll_data, 1, MESSAGE_INCOME_TIMEOUT);

	if (poll_res == -1) {
		logger << Logger::critical << "Failed to poll socket" << std::endl;
        throw std::runtime_error("Poll error");
    } else if (poll_res == 0) {
		logger << Logger::warning << "No new message received within timeout, removing connection" << std::endl;
		throw ConnectionClosedException();
	}

	return this->recieve();
}
std::string Connection::recieve() const {
	Logger logger("Connection::recieve");
	std::array<char, BUFFER_SIZE> buffer;
	ssize_t data_size = 0;

	data_size = recv(*this, buffer.data(), BUFFER_SIZE, 0); 
	logger << Logger::debug << "Received " << data_size << " bytes" << std::endl;

	if (data_size < 0) {
		logger << Logger::critical << "Failed to receive data" << std::endl;
		throw std::runtime_error("Failed to recieve data");
	}
	else if (data_size == 0) {
		logger << Logger::warning << "Connection closed by server" << std::endl;
		throw ConnectionClosedException();
	}

	std::string result(buffer.data(), (size_t)data_size);
	logger << "Received message: " << result << std::endl;

	return result;
}

void Connection::sendMessage(const std::string &message) const {
	Logger logger("Connection::sendMessage");
	
	logger << Logger::debug << "Sending message: " << message << std::endl;
	ssize_t sent = send(*this, message.c_str(), message.length(), 0);
	
	if (sent == -1) {
		logger << Logger::critical << "Failed to send message" << std::endl;
		throw std::runtime_error("Message sending error");
	}
	else if (sent == 0) {
		logger << Logger::warning << "Connection closed by server" << std::endl;
		throw ConnectionClosedException();
	}
	logger << Logger::debug << "Message sent successfully" << std::endl;
}



#include <unistd.h>
Connection::~Connection() {
	Logger logger("Connection::~Connection()");
	
	logger << Logger::important << "Closing connection with ID: " << this->id << std::endl;
	close(this->sd);
}
