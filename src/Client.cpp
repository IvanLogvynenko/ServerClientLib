#include "Client.hpp"

Client::Client(Client &other) {
	if (this == &other)
		return;

	this->connection = other.connection;
}
Client& Client::operator=(Client& other) {
	if (this == &other)
		return other;

	this->connection = other.connection;
	return *this;
}

void Client::connectTo(std::string ip, std::string port) {
	Logger logger("Client::connectTo");
	logger << Logger::important << "Connecting to " << ip << ":" << port << std::endl;
	struct addrinfo hints;
	struct addrinfo* res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (int status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res); status != 0) {
		freeaddrinfo(res);
		throw std::runtime_error("Getaddrinfo error: " + std::string(gai_strerror(status)));
	}
	logger << Logger::debug << "Got info for " << ip << ":" << port << std::endl;

	int socketd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socketd == -1) {
		freeaddrinfo(res);
		throw std::runtime_error("Socket error " + std::string(strerror(errno)));
	}
	logger << Logger::debug << "Successfully opened socket" << std::endl;

	int attempt = 0;
	while (attempt++ < MAX_CONNECTION_ATTEMPTS && connect(socketd, res->ai_addr, res->ai_addrlen) == -1)
		logger << Logger::error << "Connection failed | attempt " << attempt << std::endl;
	
	if (attempt == MAX_CONNECTION_ATTEMPTS) {
		logger << Logger::critical << "Failed to connect to server after " 
			<< MAX_CONNECTION_ATTEMPTS << " attempts" << std::endl;
		freeaddrinfo(res);
		throw std::runtime_error("Impossible to connect to server");
	}
	
	Connection *connection = new Connection(socketd);
	this->connection = connection;
	logger << Logger::important << "Successful connection to " << ip << ":" << port << std::endl;
	freeaddrinfo(res);

}
void Client::connectTo(std::string ip, uint16_t port) {
	this->connectTo(ip, std::to_string(port));	
}

void Client::disconnect() {
    if (this->connection) { 
		this->sendMessage(Connection::CLOSE_MESSAGE);
		
    	delete this->connection;
    	this->connection = nullptr;
	}
	Logger("Client::disconnect") << Logger::important << "Disconnected from server" << std::endl;
}



void Client::sendMessage(std::string data) {
	Logger logger("Client::sendMessage");
	if (!this->connection)
		logger << Logger::critical << "Cannot send message: No active connection" << std::endl;
		
	try {
		this->connection->sendMessage(data);
	} catch (ConnectionClosedException& e) {
		logger << Logger::error << "Connection was closed" << std::endl;

		delete this->connection;
		this->connection = nullptr;
	}
}
void Client::sendMessage(const char * data) {
	this->sendMessage(std::string(data));
}

std::string Client::recieve() {
	try {
		return this->connection->awaitNewMessage();
	} catch (ConnectionClosedException& e) {
		Logger("Client::recieve") << Logger::error << "Connection was closed" << std::endl;

		delete this->connection;
		this->connection = nullptr;
		throw e;
	}
}



Client::~Client() {
	if (this->connection)
		delete this->connection;
}
