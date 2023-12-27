#include "debugKit/basic.hpp"

#include "Client/Client.hpp"

int main(const int argv, const char** argc) {
    if (argv != 3) {
        std::cout << "Usage " << argc[0] << " [adress] [port]";
        return 1;
    }
    ILOG("Start");

    Client client = Client();
    client.connectTo(argc[1], argc[2]);
    client.disconnect();
    ILOG("End");
}