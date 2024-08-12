#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <cstdint>

#include <stdexcept>

#include <array>
#include <string>
#include <sstream>

#include <unistd.h>
#include <sys/socket.h>
	
namespace server_client
{
	class Connection {
		private:
			static unsigned int ID;
		protected:
			int sd;
			unsigned int id;

			std::string backlog;
		public:
			Connection(int sd);
			Connection(const Connection&);
			~Connection();
			Connection& operator=(const Connection&);



			inline unsigned int getId() const { return this->id; }
			inline int getSocket() const { return this->sd; }
			inline operator int() const { return this->sd; }
			inline bool isValid() const { return this->sd != -1 && this->sd != 0; }

			const static std::string CLOSE_MESSAGE;
			const static std::string SERVER_STOP;
	};

	class ConnectionHashFunction {
	public:
		size_t operator() (const server_client::Connection& connection) const {
			return std::hash<int>{}(connection.getSocket());
		}
	};


	class ConnectionClosedException : public std::exception {
	public:
		ConnectionClosedException() {}
		virtual const char* what() const noexcept override { return ""; }
	};

} // namespace server_client

#endif //!CONNECTION_HPP
