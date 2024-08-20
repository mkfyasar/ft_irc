#include "../include/Server.hpp"
#include "../include/Client.hpp"

#define serverIsRunning 1

int main(int argc, char **argv){

    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    
    Server server(atoi(argv[1]),argv[2]);
    server.initialize();
    server.run();    
}