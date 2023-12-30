#include "debugKit/basic.hpp"

#include "Client/Client.hpp"

#ifndef DEFAULT_PORT
    #define DEFAULT_PORT "11999"
#endif // !DEFAULT_PORT

int main(const int argv, const char** argc) {
    if (argv != 3) {
        std::cout << "Usage " << argc[0] << " [adress] [port]\n";
        std::cout << "Using default port" << DEFAULT_PORT << std::endl;
    }
    ILOG("Start");

    Client client = Client();
    client.connectTo(argc[1], (argv == 2) ? DEFAULT_PORT : argc[2]);
    
    client.sendMessage("Hello");

    ILOG("End");
}