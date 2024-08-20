#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1000
#endif // !BUFFER_SIZE

#ifndef MESSAGE_INCOME_TIMEOUT
#define MESSAGE_INCOME_TIMEOUT 10000
#endif //!MESSAGE_INCOME_TIMEOUT

#include <stdexcept>

#include <array>
#include <string>
#include <memory>

#include <atomic>
#include <thread>
#include <functional>
	
class Connection {
	private:
		static unsigned int ID;
	protected:
		int sd;
		unsigned int id;

		std::string recieve() const;
	public:
		Connection(int sd);
		Connection(const Connection&) = delete;
		~Connection();
		Connection& operator=(const Connection&) = delete;

		std::string awaitNewMessage() const;

		void sendMessage(const std::string& message) const;

		inline unsigned int getId() const { return this->id; }
		inline int getSocket() const { return this->sd; }
		inline operator int() const { return this->sd; }
		inline bool isValid() const { return this->sd != -1 && this->sd != 0; }

		const static std::string CLOSE_MESSAGE;
		const static std::string SERVER_STOP;
};

class ConnectionClosedException : public std::exception {
public:
	ConnectionClosedException() {}
	virtual const char* what() const noexcept override { return "Connection is closed"; }
};

#endif //!CONNECTION_HPP
