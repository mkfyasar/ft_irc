#include "../include/Server.hpp"


int main(int argc, char **argv){

    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    
    Server deneme(atoi(argv[0]),argv[2]);
    deneme.run();
}