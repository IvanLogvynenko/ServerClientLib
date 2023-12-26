// #define EXECUTABLE_NAME "SERVER"
#include "debugKit/basic.hpp"

#include "Server/Server.hpp"

#ifndef DEFAULT_PORT
    #define DEFAULT_PORT 11999
#endif // !DEFAULT_PORT

int main(const int argv, const char** argc) {
    if (argv != 2) {
        std::cout << "Usage: " << argc[0] << "[" << argc[0] << "]" << "\n";
        std::cout << "Hosting on a basic port: " << DEFAULT_PORT << "\n";
        std::cout << "Hint: <You can set default port with DEFAULT_PORT header while building>\n";
    }
    ILOG("Start");
    LOG("Created server");
    Server server = Server();
    if (argv != 2) 
        server.host(DEFAULT_PORT);
    else 
        server.host(argc[1]);
    LOG("Hosted on a port " << server.getPort());

    server.allowConnection();

    ILOG("End");
}