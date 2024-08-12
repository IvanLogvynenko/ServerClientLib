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
	struct addrinfo hints;
	struct addrinfo* res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (int status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res); status != 0) {
		freeaddrinfo(res);
		throw std::runtime_error("Getaddrinfo error: " + std::string(gai_strerror(status)));
	}

	int socketd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socketd == -1) {
		freeaddrinfo(res);
		throw std::runtime_error("Socket error " + std::string(strerror(errno)));
	}

	int attempt = 0;
	while (attempt++ < MAX_CONNECTION_ATTEMPTS && connect(socketd, res->ai_addr, res->ai_addrlen) == -1)
		std::cerr << "Connection failed | attempt " << attempt << std::endl;
	
	if (attempt == MAX_CONNECTION_ATTEMPTS) {
		freeaddrinfo(res);
		throw std::runtime_error("Impossible to connect to server");
	}
	
	Connection *connection = new Connection(socketd);
	this->connection = connection;
	freeaddrinfo(res);

}
void server_client::Client::connectTo(std::string ip, uint16_t port) {
	this->connectTo(ip, std::to_string(port));	
}

void server_client::Client::disconnect() {
	if (this->connection)
	    this->sendMessage(Connection::CLOSE_MESSAGE);
	delete this->connection;
	this->connection = nullptr;
}



void server_client::Client::sendMessage(std::string data) {
	if (!this->connection)
		std::cerr << "Cannot send message: No active connection" << std::endl;
	
	ssize_t sent = send(*this, data.c_str(), data.length(), 0);
	
	if (sent == -1) {
		throw std::runtime_error("Message sending error");
	}
	else if (sent == 0)
		delete this->connection;
}
void server_client::Client::sendMessage(const char * data) {
	this->sendMessage(std::string(data));
}

std::string server_client::Client::recieve() {
	std::array<char, BUFFER_SIZE> buffer;
	ssize_t data_size = 0;

	data_size = recv(*this, buffer.data(), BUFFER_SIZE, 0); 

	if (data_size < 0) 
		throw std::runtime_error("Failed to recieve data");
	else if (data_size == 0) 
		throw server_client::ConnectionClosedException();

	std::string result(buffer.data(), (size_t)data_size);

    if (result == Connection::SERVER_STOP) {
		delete this->connection;
		return "";
	}

	return result;
}



server_client::Client::~Client() {
	if (this->connection)
		delete this->connection;
}
