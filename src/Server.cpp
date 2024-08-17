#include "Server.hpp"
using namespace server_client;

Server::Server(int sd, int port) : 
	socketd(sd), 
	port(port) {}
server_client::Server &server_client::Server::operator=(Server &other) {
	if (this != &other) 
		return other;

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
server_client::Server* Server::host(uint16_t port) {
	Logger logger("Server::host");
	int socket = open_socket();
	bind_socket(socket, port);
	listen_socket(socket);
	logger << Logger::important << "Server started on port " << port <<  std::endl;
	logger << Logger::debug << "Server socket is " << socket << std::endl;

	return new Server(socket, port);
}



std::string recieve(Connection *conn) {
	Logger logger("Server::recieve");
	std::array<char, BUFFER_SIZE> buffer;
	ssize_t data_size = 0;

	logger << Logger::info << "Awaiting data" << std::endl;
	data_size = recv(*conn, buffer.data(), BUFFER_SIZE, 0); 

	if (data_size < 0) 
		throw std::runtime_error("Failed to recieve data");
	else if (data_size == 0) { 
		logger << Logger::error << "Connection closed by client" << std::endl;
		throw server_client::ConnectionClosedException();
	}

	std::string result(buffer.data(), (size_t)data_size);
	logger << Logger::debug << "Received data: " << result << std::endl;

    if (result == Connection::CLOSE_MESSAGE) {
		logger << "Recieved close message, closing connection" << std::endl;
		throw server_client::ConnectionClosedException();
	}

	return result;
}

void server_client::Server::sendMessage(const char *data, Connection* conn) {
	this->sendMessage(std::string(data), conn);
}
void server_client::Server::sendMessage(std::string data, Connection* conn) {
	Logger logger("Server::sendMessage");
	ssize_t sent = send(*conn, data.c_str(), data.length(), 0);
	logger << Logger::debug << "Sent data: " << data << "(" << data.length() << "bytes)" << std::endl;
	
	if (sent == -1) {
		logger << Logger::critical << "Failed to send data";
		throw std::runtime_error("Message sending error");
	}
	else if (sent == 0) {
		logger << Logger::debug << "Connection closed by client" << std::endl;
		removeSocket(conn);
	}
}

void server_client::Server::respond(const char *data) {
	respond(std::string(data));
}
void server_client::Server::respond(std::string data) {
	if (last_used_connection)
		sendMessage(data, last_used_connection);
	else throw std::runtime_error("No active connection to respond to");
}

std::string server_client::Server::awaitNewMessage(Connection* conn) {
	Logger logger("Server::awaitNewMessage");
	pollfd poll_data = {*conn, POLLIN, 0 };
	logger << Logger::important << "Awaiting new message" << std::endl;
	logger << Logger::debug << "Timeout: " << MESSAGE_INCOME_TIMEOUT << "ms" << std::endl;
	int poll_res = poll(&poll_data, 1, MESSAGE_INCOME_TIMEOUT);

	if (poll_res == -1) {
		logger << Logger::critical << "Failed to poll socket" << std::endl;
        throw std::runtime_error("Poll error");
    } else if (poll_res == 0) {
		logger << Logger::warning << "No new message received within timeout, removin connection" << std::endl;
		removeSocket(conn);
		return "";
	}

	this->last_used_connection = conn;
	try {
		return recieve(conn);
	}
	catch (server_client::ConnectionClosedException &e) {
		removeSocket(conn);
		return "";
	}

}



std::thread *server_client::Server::startMessageIncomeHandlingThread(
		Connection *conn, std::function<void(std::string &, Connection *)> on_recieve) {
	Logger logger("Server::startMessageIncomeHandlingThread");
	logger << Logger::important << "Starting message income handling thread for connection " << *conn << std::endl;

	return std::move(new std::thread([this, conn, on_recieve]() {
		Logger logger("Server.messageHandlingThread");
		logger << Logger::debug << "Message income handling thread started for connection " << *conn <<  std::endl;

		while (!this->message_handler_stop_trigger) {
			if (this->income_message_handlers.find(*conn) == this->income_message_handlers.end())
			    break;

			std::string message = this->awaitNewMessage(conn);
			if (message.empty())
				break;
			if (on_recieve)
				on_recieve(message, conn);
		}

		logger << Logger::debug << "Message income handling thread for connection " << *conn << " stopped" << std::endl;
	}));
}



void server_client::Server::awaitNewConnection(std::function<Connection*(Connection*)> on_connect) {
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
		conn = on_connect(conn);

	if (conn && conn->isValid()) {
		std::lock_guard<std::mutex> lock(connections_lock);
		connections.push_back(std::move(conn));
		if (!this->message_handler_stop_trigger) {
			std::lock_guard<std::mutex> other_lock(message_handlers_lock);
			income_message_handlers[*conn] = this->startMessageIncomeHandlingThread(conn, this->on_recieve);
			logger << "Connection added to message handlers" << std::endl;
		}
	}
}

void server_client::Server::removeSocket(Connection *conn) {
	if (this->last_used_connection == conn)
		this->last_used_connection = nullptr;

	for (size_t i = 0; i < connections.size(); ++i) {
		if (connections[i]->getId() == conn->getId()) {
			std::lock_guard<std::mutex> lock(connections_lock);
			connections.erase(connections.begin() + i);
		}
	}
	std::lock_guard<std::mutex> other_lock(message_handlers_lock);
	income_message_handlers[*conn]->detach();
	income_message_handlers.erase(*conn);

	delete conn;
}



void server_client::Server::startConnectionHandling(std::function<Connection*(Connection*)> on_connect) {
	Logger logger("Server::startConnectionHandling");
	if (!this->connection_handler_stop_trigger)
	    return;
	logger << Logger::important << "Starting connection handling thread" << std::endl;
	this->connection_handler = std::move(
		new std::thread([&](std::function<Connection*(Connection*)> on_connect) {
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
				if (!this->message_handler_stop_trigger) {
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
				conn = on_connect(conn);

			if (conn && conn->isValid()) {
				std::lock_guard<std::mutex> lock(connections_lock);
				connections.push_back(conn);
				if (!this->message_handler_stop_trigger) {
					std::lock_guard<std::mutex> other_lock(message_handlers_lock);
					income_message_handlers[*conn] = this->startMessageIncomeHandlingThread(conn, this->on_recieve);
					logger << "Connection added to message handlers" << std::endl;
				}
			}
		}
		logger << Logger::debug << "Connection handling thread stopped" << std::endl;
	}, std::move(on_connect)));
}
void server_client::Server::stopConnectionHandling() {
	this->connection_handler_stop_trigger = true;
    if (this->connection_handler.load())
        this->connection_handler.load()->join();
}



void server_client::Server::startMessageIncomeHandling(std::function<void(std::string &, Connection *)> on_recieve) {
	if (!this->message_handler_stop_trigger)
	    return;
	Logger logger("Server::startMessageIncomeHandling");
	this->on_recieve = std::move(on_recieve);
	this->message_handler_stop_trigger = false;

	logger << Logger::important << "Starting message income handling threads" << std::endl;
	
	std::lock_guard<std::mutex> lock(message_handlers_lock);
	for (auto connection : this->getConnections()) 
		income_message_handlers[*connection] = this->startMessageIncomeHandlingThread(connection, this->on_recieve);
}
void server_client::Server::stopMessageIncomeHandling() {
	this->message_handler_stop_trigger = true;
	for (auto &[_, handler] : income_message_handlers) {
        handler->join();
        delete handler;
    }
}



server_client::Server::~Server() {
	if (this->connection_handler.load()) {
		connection_handler.load()->join();
		delete this->connection_handler;
	}

    std::lock_guard<std::mutex> handlers_lock(message_handlers_lock);
	if (this->income_message_handlers.size())
		for (auto &[_, handler] : income_message_handlers) {
			handler->join();
            delete handler;
		}

	std::lock_guard<std::mutex> lock(connections_lock);
	for (auto &conn : connections) {
		this->sendMessage(Connection::SERVER_STOP, conn);
	    delete conn;
	}
}
