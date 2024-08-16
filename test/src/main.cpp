#include "../include/Server.hpp"

#define BUFFER_SIZE 1024

int main(int argc, char **argv){


    char buffer[BUFFER_SIZE] = {0};


    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    Server deneme(atoi(argv[1]), argv[2]);
    int fd = deneme.createSocket();
    deneme.setSocket(fd);
    deneme.createBind(fd, atoi(argv[1]));
    deneme.createListen(fd);
    std::cout << "Sunucu >>>" << deneme.getPortNumber() << "<<< Portunda Dinliyor..." << std::endl;
    if(deneme.setNonBlocking(fd) == -1){
        std::cerr << "Soket Bloknamayan Moda Alinamadi!!!" << std::endl;
    }else
        printf("Soket Bloknamayan Moda Alindi\n");

    while(true){

       int newSocket = deneme.createAccept(fd);
       if(newSocket == -1){
            if(errno == EWOULDBLOCK || errno == EAGAIN){
                std::cout << "Şu anda bekleyen bağlantı yok, diğer işler yapılıyor..." << std::endl;
                sleep(5);
                continue;
            }else{
                std::cerr << "Accept Hatasi: " << strerror(errno) << std::endl;
                break;
            }
       }
         std::cout << "Yeni bağlantı kabul edildi" << std::endl;
    }

}