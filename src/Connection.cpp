#include "Connection.hpp"

u_int8_t Connection::id = 1;
const Connection Connection::empty = Connection();

/// @brief Basic constructor
/// @param socket_fd 
Connection::Connection(int socket_fd) :
    m_socket_fd(socket_fd)
{
    m_isEmpty = socket_fd == -1;
    if (m_isEmpty) 
        ID = 0;
    else {
        ID = id++; 
        LOG("Connection " << (int)this->ID << " opened");
    }
    
}

Connection::~Connection() {
    close(this->m_socket_fd);
    if (this->ID != 0) {
        LOG("Connection " << (int)this->ID << " closed");
    }
}

Connection & Connection::operator=(const Connection& other)
{
    if (this != &other) {
        this->ID = other.ID;
        this->m_socket_fd = other.m_socket_fd;
        this->m_isEmpty = other.m_isEmpty;
    }
    return *this;
}

bool Connection::isEmpty() const
{
    return this->m_isEmpty;
}

// now important functions
bool Connection::checkValidity() const
{
    return this->m_socket_fd != -1 
        && !this->m_isEmpty;
}
u_int8_t Connection::getID() const
{
    return this->ID;
}
Connection::operator int() const
{
    return this->m_socket_fd;
}

const Connection &Connection::operator<<(std::string &data) const
{
    LOG("Sending message: " << data);
	if (send(this->m_socket_fd, (data.append("<END>")).c_str(), data.length(), 0) == -1) {
		EL("Message sending failed");
		throw std::runtime_error("Message sending error");
	}
    return *this;
}

const Connection &Connection::operator>>(std::string &data) const
{
    data = this->recieve();
    return *this;
}

std::string Connection::recieve() const
{
    std::array<char, BUFFER_SIZE> buffer;
	size_t data_size = 0;
	std::string result = "";
	while (true) {
		data_size = (size_t)recv(this->m_socket_fd, buffer.data(), BUFFER_SIZE, 0); 
		if (data_size == 0) {
			EL("Failed to recieve data from " << this->m_socket_fd);
			throw std::runtime_error("Failed to recieve data");
		}
		std::string current(buffer.data(), data_size);
		if (current.find("<END>") != std::string::npos) {
			result.append(current.erase(current.find("<END>"), 5));
            break;
        }
		result.append(current);
	}
	
	LOG("Recieved message from " << this->m_socket_fd);
	LOG("Message: " << result);
	return result;
}