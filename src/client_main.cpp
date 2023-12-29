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

    const char* port = argc[2];
    if (argv == 2)
        port = DEFAULT_PORT;

    Client client = Client();
    client.connectTo(argc[1], port);
    client.sendMessage("Hello");
    client.disconnect();
    ILOG("End");
}