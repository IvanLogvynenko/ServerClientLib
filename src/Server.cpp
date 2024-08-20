#include "Server.hpp"

Server::Server(int sd, int port) : 
	socketd(sd), 
	port(port) {}
Server &Server::operator=(Server &other) {
	if (this != &other) 
		return *this;

	if (connections.size() > 0) 
		throw std::runtime_error("Cannot copy server with active connections\n"
			"Argument passing should be by pointer");
	
	socketd = other.socketd;
	port = other.port;

	return *this;
}

//* a bunch of utility functions so that host func doesn't look ugly *//
int open_socket() {
	Logger logger("Server::host.open_socket");
	int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socket_fd == -1)
		throw std::runtime_error("Socket opening error");

	int yes = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	logger << "Server socket opened successfully" << std::endl;

	return socket_fd;
}
void bind_socket(int socket_fd, uint16_t port) {
	Logger logger("Server::host.bind_socket");
	struct sockaddr_in socketAddr;
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(port);
	socketAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr*)&socketAddr, sizeof(socketAddr))) {
		logger << Logger::critical << "Failed to bind socket" << std::endl;
		close(socket_fd);
		throw std::runtime_error("Caught error while binding");
	}
	logger << "Server socket bound successfully" << std::endl;
}
void listen_socket(int socket_fd) {
	Logger logger("Server::host.listen_socket");
	if (listen(socket_fd, LISTEN_BACKLOG)) {
		logger << Logger::critical << "Failed to listen on socket" << std::endl;
		close(socket_fd);
		throw std::runtime_error("Failed to set socket to listening state");
	}
	logger << "Server socket listening successfully" << std::endl;
}
//* host? *//
Server* Server::host(uint16_t port) {
	Logger logger("Server::host");
	int socket = open_socket();
	bind_socket(socket, port);
	listen_socket(socket);
	logger << Logger::important << "Server started on port " << port <<  std::endl;
	logger << Logger::debug << "Server socket is " << socket << std::endl;

	return new Server(socket, port);
}



void Server::sendMessage(const char *data, Connection* conn) {
	this->sendMessage(std::string(data), conn);
}
void Server::sendMessage(std::string data, Connection* conn) {
	Logger logger("Server::sendMessage");
	
	try {
		logger << "Sending message: " << data << std::endl;
		conn->sendMessage(data);
	} catch (ConnectionClosedException& e) {
		logger << Logger::error << "Connection was closed" << std::endl;

		removeConnection(conn);
	}
}

void Server::respond(const char *data) {
	respond(std::string(data));
}
void Server::respond(std::string data) {
	if (last_used_connection)
		sendMessage(data, last_used_connection);
	else throw std::runtime_error("No active connection to respond to");
}

std::string Server::recieve(Connection* conn) {
	Logger logger("Server::awaitNewMessage");
	
	try {
		logger << "Awaiting new message for connection " << *this << std::endl;

		auto result = conn->awaitNewMessage();

		if (result == Connection::CLOSE_MESSAGE) {
			logger << "Recieved close message, closing connection" << std::endl;
			throw ConnectionClosedException();
		}

		return result;
	}
	catch (ConnectionClosedException& e) {
		removeConnection(conn);
		throw e;
	}    
}



void Server::startMessageIncomeHandlingThread(Connection *conn, std::function<void(const std::string &, const Connection *)> on_recieve) {
	std::thread thread([this, on_recieve, conn]() {
	    int connectionID = conn->getId()
		Logger logger("Connection.messageHandlingThread");
		logger << Logger::debug << "Message income handling thread started for connection " << connectionID <<  std::endl;
		while (this->start_message_income_handling_on_connect) {
			try {
				std::string message = this->recieve(conn);
				if (on_recieve)
					on_recieve(message, conn);
			} catch (ConnectionClosedException& e) {
				logger << Logger::error << "Connection was closed, stopping message income handler" << std::endl;
				break;
			}
		}
		logger << Logger::debug << "Message income handling thread for connection " << connectionID << " stopped" << std::endl;
	});
    thread.detach();
	// Detaching since there is a problem of joining thread
	// The thread is guaranteed to break without causing errors because recieve() will throw ConnectionCLosedException
	// if you know how to fix, do it!
	// Hours of trying to fix: 25h
}



void Server::awaitNewConnection(std::function<void(const Connection*)> on_connect) {
	Logger logger("Server::awaitNewConnection");
	pollfd poll_data = { *this, POLLIN, 0 };
	logger << Logger::important << "Awaiting new connection" << std::endl;
    logger << Logger::debug << "Timeout: " << CONNECTION_TIMEOUT << "ms" << std::endl;
	int poll_res = poll(&poll_data, 1, CONNECTION_TIMEOUT);

	if (poll_res == -1) {
		logger << Logger::critical << "Failed to poll socket" << std::endl;
        throw std::runtime_error("Poll error");
	}
    else if (poll_res == 0) {
		logger << Logger::warning << "No new connection received within timeout" << std::endl;
		throw std::runtime_error("Timeout while waiting for new connection");
	}

	int new_sd = accept(socketd, nullptr, 0);
	if (new_sd == -1) 
	    throw std::runtime_error("Accept error");
	logger << Logger::important << "New connection accepted: " << new_sd << std::endl;
	
	Connection* conn = new Connection(new_sd);
	if (on_connect)
		on_connect(conn);

	std::lock_guard<std::mutex> lock(connections_lock);
	connections.push_back(std::move(conn));
	
	if (this->start_message_income_handling_on_connect)
		this->startMessageIncomeHandlingThread(conn, this->on_recieve);
}

void Server::removeConnection(Connection *conn) {
	Logger logger("Server::removeSocket");
	if (this->last_used_connection == conn)
		this->last_used_connection = nullptr;

	logger << Logger::debug << "Removing connection " << *conn << std::endl;
	for (size_t i = 0; i < connections.size(); ++i) {
		if (connections[i]->getId() == conn->getId()) {
			logger << Logger::debug << "Connection removed from message handlers" << std::endl;
			std::lock_guard<std::mutex> lock(connections_lock);
			connections.erase(connections.begin() + i);
			break;
		}
	}

	delete conn;
}



void Server::startConnectionHandling(std::function<void(const Connection*)> on_connect) {
	Logger logger("Server::startConnectionHandling");
	if (!this->connection_handler_stop_trigger)
	    return;
	logger << Logger::important << "Starting connection handling thread" << std::endl;
	this->connection_handler = new std::thread([this, on_connect]() {
		Logger logger("Server.connectionHandlingThread");

		logger << Logger::debug << "Connection handling thread started" << std::endl;
		this->connection_handler_stop_trigger = false;
		while (!this->connection_handler_stop_trigger) {
			pollfd poll_data = { *this, POLLIN, 0 };
			logger << Logger::important << "Awaiting new connection" << std::endl;
            logger << Logger::debug << "Timeout: " << CONNECTION_TIMEOUT << "ms" << std::endl;
			int poll_res = poll(&poll_data, 1, CONNECTION_TIMEOUT);

			if (poll_res == -1)
				throw std::runtime_error("Poll error");
			else if (poll_res == 0) {
				if (this->getConnections().size() == 0) {
					logger << "No connections, stoppping..." << std::endl;
					break;
				}
				if (this->start_message_income_handling_on_connect) {
					logger << Logger::warning << "No new connection, mesage handling thread is running, continuing..." << std::endl;
				    continue;
				}
				else {
					logger << Logger::important << "No new connection and no thread to detect disconnection, stopping...";
					break;
				}
			}

			int new_sd = accept(socketd, nullptr, 0);
			if (new_sd == -1) 
				throw std::runtime_error("Accept error");
			logger << Logger::important << "New connection accepted: " << new_sd << std::endl;
			
			Connection* conn = new Connection(new_sd);
			if (on_connect)
				on_connect(conn);

			{std::lock_guard<std::mutex> lock(connections_lock);
			connections.push_back(conn);}

			logger << Logger::debug << "Adding connection to message handlers, should start=" << this->start_message_income_handling_on_connect << std::endl;
			if (this->start_message_income_handling_on_connect) {
				this->startMessageIncomeHandlingThread(conn, this->on_recieve);
				logger << Logger::debug << "Connection started message handling" << std::endl;
			}
		}
		logger << Logger::debug << "Connection handling thread stopped" << std::endl;
	});
}
void Server::stopConnectionHandling() {
	this->connection_handler_stop_trigger = true;
    if (this->connection_handler.load()) {
        this->connection_handler.load()->join();
		delete this->connection_handler;
	}
}



void Server::startMessageIncomeHandling(std::function<void(const std::string &, const Connection*)> on_recieve) {
	if (this->start_message_income_handling_on_connect)
	    return;

	Logger logger("Server::startMessageIncomeHandling");
	logger << Logger::important << "Starting message income handling threads" << std::endl;

	this->start_message_income_handling_on_connect = true;

	logger << Logger::debug << "Message income handling threads started" << (on_recieve ? " with callback" : "") << std::endl;

	this->on_recieve = std::move(on_recieve);

	std::lock_guard<std::mutex> lock(connections_lock);
	for (auto &conn: this->getConnections())
		this->startMessageIncomeHandlingThread(conn, this->on_recieve);
}
void Server::stopMessageIncomeHandling() {
	if (!this->start_message_income_handling_on_connect)
		return;
	
	this->start_message_income_handling_on_connect = false;
}



Server::~Server() {
	if (this->connection_handler.load()) {
		connection_handler.load()->join();
		delete this->connection_handler;
	}

	std::lock_guard<std::mutex> lock(connections_lock);
	for (auto &conn : connections) {
		this->sendMessage(Connection::SERVER_STOP, conn);
	    delete conn;
	}
}
