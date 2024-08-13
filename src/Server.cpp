#include "Server.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <arpa/inet.h> // Add this line to include the inet_addr function

Server::Server(int port, const std::string& password) 
    :_port(port), _password(password), _serverSocket(-1){
    std::cout << "Server Constructer Called!" << std::endl;
}
Server::~Server() {
    stop();
    std::cout << "Server Destructor Called!" << std::endl;
}

void Server::stop(){
    for (std::vector<int>::iterator it = _clientSockets.begin(); it != _clientSockets.end(); ++it){
        close(*it);
    }
    if (_serverSocket != -1){
        close(_serverSocket);
    }

}
void Server::start(){
    setupServerSocket();
    std::cout << "Server started on port" << std::endl;
    std::cout << "Server setup completed. Press Ctrl+C to exit." << std::endl;
    while (true) {
    }
    
}

    void Server::setupServerSocket(){

        try {
            std::cout << "Setup Server Socket Running!" << std::endl;   

            //1. Socket olusturma
            _serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
            if(_serverSocket == -1){
                throw std::runtime_error("Failed to create socket!");
            }

            //2.Socket Secenekleri Ayarlama
            int opt = 1;
            if(setsockopt(_serverSocket,SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
                throw std::runtime_error("Failed to set socket options!");
            }

            //3.Sunucu Adresi Tanımlama
            struct sockaddr_in server_addr;
            server_addr.sin_family = AF_INET; // Adres ailesi af ınet ıpv4 için
            server_addr.sin_port = htons(_port); // Sunucu Portu
            server_addr.sin_addr.s_addr = INADDR_ANY; // Sunucu İp Adresi


            //4.Soketi Bağlama
            if(bind(_serverSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
                throw std::runtime_error("Failed to bind socket address!");
            }

            //5.Dinlemeye Baslama
            if(listen(_serverSocket,SOMAXCONN) < 0){
                throw std::runtime_error("Failed to listen on socket!");
            }

            //6.Bloklanamayan Mod ayarı
            int flags = fcntl(_serverSocket, F_GETFL, 0);
            if(flags == -1){
                throw std::runtime_error("Failed to get socket flags");
            }
            if(fcntl(_serverSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
                throw std::runtime_error("Failed to set socket to non-blocking mode");
            }


        
        } catch (const std::exception& e) {
            std::cerr << "Error in setupServerSocket: " << e.what() << std::endl;
            // Temizleme işlemleri veya ek hata yönetimi buraya gelebilir
            throw; // Hatayı yukarı seviyeye ilet

        }
        std::cout << "Server is set up and listening on port ----> " << _port << std::endl;
    }
    
