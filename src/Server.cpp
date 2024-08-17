#include "../include/Server.hpp"


Server::Server(int port, const std::string& password) 
    :_portNumber(port), _password(password) {
    std::cout << "Server Constructer Called!" << std::endl;
}

Server::~Server() {
    std::cout << "Server Destructor Called!" << std::endl;
}

void Server::initSocket(){
    //1. Socket Olusturma
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd == -1){
        std::cerr << "Socket Created Failed!" << std::endl;
        return;
    } 

    //2. Socket Ayarlama /SOL_SOCKET = socket seviysei protokol /SO_REUSEADDR = ayni adres ayni port yeniden kullanim izini
    int opt = 1;
    if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        std::cerr << "Socket Set Failed!" << std::endl;
        close(socketFd);
        return;
    }

    //3.Soketi adrese ve porta baglama.
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_portNumber);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socketFd, (const sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        std::cerr << "Bind Failed!" << std::endl;
        return;
    }

    //4.Socketi Dinleme Moduna Alma
    if(listen(socketFd,10) < 0){
        std::cerr << "Listen Failed" << std::endl;
        close(socketFd);
        return;
    }
    _serverSocket = socketFd;
    //5.Soketi Bloklanamayan Moda alma
    int flags = fcntl(socketFd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    if(fcntl(socketFd,F_SETFL,flags) < 0){
        std::cerr << "Non Block Mode Failed" << std::endl;
        return;
    }
    std::cout << "Server Initialized and Listening on Port ===> " << _portNumber << std::endl;
}

void Server::setPollFd(){
    pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;
    serverPollFd.revents = 0;

    fds.push_back(serverPollFd);
}


void Server::acceptConnection(){
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    
}









void Server::run(){
    setPollFd();

    while (true){
        int ready = poll(fds.data(),fds.size(), -1);
        if(ready == -1){
            std::cerr << "Poll Failed!" << std::endl;
            continue;
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == _serverSocket) {
                    // yeni baglanti kabul edecgiz
                } else {
                    // istemciden gelen veriler.

                } 
            }
        }
    }
}