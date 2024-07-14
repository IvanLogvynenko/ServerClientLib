#ifndef SERVER_HPP
#define SERVER_HPP

#ifndef LISTEN_BACKLOG
	#define LISTEN_BACKLOG 20
#endif // !LISTEN_BACKLOG

#ifndef DEFAULT_PORT
	#define DEFAULT_PORT 37373
#endif // !DEFAULT_PORT

#ifndef HANDLING_TIMEOUT
	#define HANDLING_TIMEOUT 500
#endif // !HANDLING_TIMEOUT

//data structure for storing client information
#include <vector>
#include <string>
#include <algorithm>

//network related includes
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

//asyncrounous related includes
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

//allowing callbacks
#include <functional>
#include <unordered_map>

//enabling logging
#include "basic.hpp"

//class for managing client connections
#include "Connection.hpp"

class Server
{
private:
	int m_socket, m_port;
	Connection m_lastly_used_connection; // used to provide respond method

	std::mutex m_connections_lock;
	std::vector<Connection*> m_connections;



	std::atomic_bool m_handle_connection;
	std::function<Connection* (Connection*)> m_on_connect = nullptr;

	std::atomic_bool m_handle_message_income;
	std::function<void(std::string&, Connection*)> m_on_recieve = nullptr;
	


	std::atomic_bool m_thread_stop;

	std::mutex m_main_thread_mutex;
	std::condition_variable m_main_thread_lock;

	std::atomic<std::thread*> m_main_thread;

	std::mutex m_message_income_threads_lock;
	std::unordered_map<int, std::thread*> m_message_income_threads;
protected:
	/**
	 * methods to control "central" thread that manages threads 
	 * for both incoming connections and messages
	 */
	std::thread* startNewMessageIncomeThread(Connection* connection);

	void startEventHandler();
	void stopEventHandler();

public:
	Server();
	Server(Server&);
	Server& operator=(Server& other);

	~Server();

	//* configuration interface
	static Server host(uint16_t port = DEFAULT_PORT);

	Connection* awaitNewConnection(std::function<Connection*(Connection*)> = nullptr);
	std::string awaitNewMessage(Connection*, 
							std::function<void(std::string&, Connection*)> = nullptr);

	void startConnectionHandling(
		std::function<Connection* (Connection*)> = nullptr
	);
	void stopConnectionHandling();

	void startMessageIncomeHandling(
		std::function<void(std::string&, Connection*)> = nullptr
	);
	void stopMessageIncomeHandling();

	//* I/O interface
	void sendMessage(const char* = "", size_t = 0) const;
	void sendMessage(std::string = "", size_t = 0) const;
	void sendMessage(std::string, const Connection& connection) const;

	std::string recieveMessageFrom(size_t = 0);
	std::string recieveMessageFrom(const Connection&);

	void respond(const char* = "") const;
	void respond(std::string = "") const;

	//* getters
	operator int() const;
	int getPort() const;
	std::vector<Connection*> getConnections() const;
};

#endif // !SERVER_HPP

