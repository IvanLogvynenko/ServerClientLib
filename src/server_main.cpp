// #define EXECUTABLE_NAME "SERVER"
#include "debugKit/basic.hpp"

#include "Server/Server.hpp"

#ifndef DEFAULT_PORT
    #define DEFAULT_PORT "11999"
#endif // !DEFAULT_PORT

int main(const int argv, const char** argc) {
    ILOG("Start");
    if (argv != 2) {
        std::cout << "Usage: " << argc[0] << " [port]" << "\n";
        std::cout << "Hosting on a basic port: " << DEFAULT_PORT << "\n";
        std::cout << "Hint: <You can set default port with DEFAULT_PORT header while building>\n";
    }
    LOG("Created server");
    Server server = Server();
    if (argv != 2) 
        server.host();
    else 
        server.host(argc[1]);
    
    LOG("Hosted on a port " << server.getPort());
    LOG("Server socket fd is " << (int)server);

    server.awaitNewConnection();
    LOG("Getting ready to recieve");

    std::unique_ptr<Responce> response = server.recieveMessageFrom();
    LOG("Got message " << *response);

    ILOG("End");
}