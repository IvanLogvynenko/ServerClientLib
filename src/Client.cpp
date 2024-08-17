#include "Client.hpp"

server_client::Client::Client(server_client::Client &other) {
	if (this == &other)
		return;

	this->connection = other.connection;
}
server_client::Client& server_client::Client::operator=(Client& other) {
	if (this == &other)
		return other;

	this->connection = other.connection;
	return *this;
}

void server_client::Client::connectTo(std::string ip, std::string port) {
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
void server_client::Client::connectTo(std::string ip, uint16_t port) {
	this->connectTo(ip, std::to_string(port));	
}

void server_client::Client::disconnect() {
	Logger logger("Client::disconnect");
	if (this->connection)
	    this->sendMessage(Connection::CLOSE_MESSAGE);

	logger << Logger::important << "Disconnected from server" << std::endl;
    if (this->connection)
        close(*this);

    delete this->connection;
    this->connection = nullptr;
}



void server_client::Client::sendMessage(std::string data) {
	Logger logger("Client::sendMessage");
	if (!this->connection)
		logger << Logger::critical << "Cannot send message: No active connection" << std::endl;
	
	ssize_t sent = send(*this, data.c_str(), data.length(), 0);
	
	if (sent == -1) {
		logger << Logger::critical << "Failed to send message" << std::endl;
		throw std::runtime_error("Message sending error");
	}
	else if (sent == 0) {
		logger << Logger::warning << "Connection closed by server" << std::endl;
		delete this->connection;
		this->connection = nullptr;
	}
}
void server_client::Client::sendMessage(const char * data) {
	this->sendMessage(std::string(data));
}

std::string server_client::Client::recieve() {
	Logger logger("Client::recieve");
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
		throw server_client::ConnectionClosedException();
	}

	std::string result(buffer.data(), (size_t)data_size);
	logger << Logger::debug << "Received message: " << result << std::endl;

    if (result == Connection::SERVER_STOP) {
		logger << "Server stopped, disconnecting..." << std::endl;
		delete this->connection;
		this->connection = nullptr;
		return "";
	}

	return result;
}



server_client::Client::~Client() {
	if (this->connection)
		delete this->connection;
}
