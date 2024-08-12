#ifndef CLIENT_HPP
#define CLIENT_HPP

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1000
#endif // !BUFFER_SIZE

#ifndef MAX_CONNECTION_ATTEMPTS
#define MAX_CONNECTION_ATTEMPTS 50
#endif // !MAX_CONNECTION_ATTEMPTS

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 30000
#endif // !DEFAULT_PORT

#include <iostream>

#include <string>
#include <cstddef>

#include <netdb.h>
#include <memory.h>
#include <sys/socket.h>

#include "Connection.hpp"

namespace server_client {
	class Client {
		protected: Connection* connection;
		public:
			Client() {}
			Client(Client&);
			Client& operator=(Client&);
			~Client();

			// I/O
			void sendMessage(const char *);
			void sendMessage(std::string);

			std::string recieve();

			// net controll
			void connectTo(std::string, std::string);
			void connectTo(std::string, uint16_t = DEFAULT_PORT);
			
			void disconnect();
			
			inline operator int() { return *connection; }
	};
}

#endif // !CLIENT_HPP