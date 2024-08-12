#ifndef SERVER
#define SERVER

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1000
#endif //!BUFFER_SIZE

#ifndef LISTEN_BACKLOG
#define LISTEN_BACKLOG 50
#endif //!LISTEN_BACKLOG

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 30000
#endif //!DEFAULT_PORT

#ifndef CONNECTION_TIMEOUT
#define CONNECTION_TIMEOUT 15000
#endif //!CONNECTION_TIMEOUT

#ifndef MESSAGE_INCOME_TIMEOUT
#define MESSAGE_INCOME_TIMEOUT 10000
#endif //!MESSAGE_INCOME_TIMEOUT

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

namespace server_client
{
	class Server {
		protected:
			int socketd, port;
			Connection* last_used_connection;
			
			std::mutex connections_lock;
			std::vector<Connection*> connections;

			std::atomic_bool connection_handler_stop_trigger = true;
			std::atomic<std::thread*> connection_handler;

			std::mutex message_handlers_lock;
			std::atomic_bool message_handler_stop_trigger = true;
			std::function<void(std::string&, Connection*)> on_recieve;
			std::unordered_map<Connection, std::thread*, ConnectionHashFunction> income_message_handlers;

			Server(int, int);
		public:
			Server& operator=(Server&);
			~Server();

			static Server* host(uint16_t = DEFAULT_PORT);

			// * I/O methods
			virtual void sendMessage(const char *, Connection*);
			virtual void sendMessage(std::string, Connection*);

			virtual void respond(const char*);
			virtual void respond(std::string);

			virtual std::string awaitNewMessage(Connection*);

			//* Server actions
			virtual std::thread* startMessageIncomeHandlingThread(Connection*, std::function<void(std::string&, Connection*)> = nullptr);
			virtual void awaitNewConnection(std::function<Connection*(Connection*)> = nullptr);

            virtual void removeSocket(Connection*);

			//* Thread management
			virtual void startConnectionHandling(std::function<Connection*(Connection*)> = nullptr);
			virtual void stopConnectionHandling();

			virtual void startMessageIncomeHandling(std::function<void(std::string&, Connection*)> = nullptr);
			virtual void stopMessageIncomeHandling();

			//* geters
			inline operator int() const { return socketd; }
			inline int getPort() const { return port; }
			inline std::vector<Connection*> getConnections() const { return connections; }
	};
} // namespace server_client

#endif // !SERVER


