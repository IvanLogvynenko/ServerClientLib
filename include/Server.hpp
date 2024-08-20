#ifndef SERVER
#define SERVER

#ifndef LISTEN_BACKLOG
#define LISTEN_BACKLOG 50
#endif //!LISTEN_BACKLOG

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 30000
#endif //!DEFAULT_PORT

#ifndef CONNECTION_TIMEOUT
#define CONNECTION_TIMEOUT 15000
#endif //!CONNECTION_TIMEOUT

#include <iostream>

#include <string>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <unordered_map>

#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <thread>
#include <mutex>
#include <atomic>

#include "Connection.hpp"

#include "Logger.hpp"

class Server {
	protected:
		int socketd, port;
		Connection* last_used_connection;
		
		std::mutex connections_lock;
		std::vector<Connection*> connections;

		std::atomic_bool connection_handler_stop_trigger = true;
		std::atomic<std::thread*> connection_handler;

		// std::mutex message_handlers_lock;
		std::function<void(const std::string&, const Connection*)> on_recieve;
		std::atomic_bool start_message_income_handling_on_connect = false;
		// std::unordered_map<Connection, std::thread*, ConnectionHashFunction> income_message_handlers;

		Server(int, int);
	public:
		Server& operator=(Server&);
		virtual ~Server();

		static Server* host(uint16_t = DEFAULT_PORT);

		// * I/O methods
		virtual void sendMessage(const char *, Connection* conn);
		virtual void sendMessage(std::string, Connection* conn);

		virtual void respond(const char*);
		virtual void respond(std::string);

		virtual std::string recieve(Connection* conn);

		//* Server actions
		virtual void startMessageIncomeHandlingThread(Connection  *conn, std::function<void(const std::string&, const Connection*)> = nullptr);

		virtual void awaitNewConnection(std::function<void(const Connection*)> = nullptr);

		virtual void removeConnection(Connection* conn);

		//* Thread management
		virtual void startConnectionHandling(std::function<void(const Connection*)> = nullptr);
		virtual void stopConnectionHandling();

		virtual void startMessageIncomeHandling(std::function<void(const std::string&, const Connection*)> = nullptr);
		virtual void stopMessageIncomeHandling();

		//* geters
		inline operator int() const { return socketd; }
		inline int getPort() const { return port; }
		inline std::vector<Connection*> getConnections() const { return connections; }
};

#endif // !SERVER
