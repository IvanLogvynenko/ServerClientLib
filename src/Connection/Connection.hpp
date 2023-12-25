#pragma once
class Connection
{
private:
    int m_socket_fd;
public:
    Connection(int socket_fd);
    ~Connection();
    operator int() const;
};


