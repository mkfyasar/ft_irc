#ifndef SERVER_HPP
#define SERVER_HPP

#include "../include/SignalHandler.hpp"
#include "../include/Client.hpp"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <cstdlib>
#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <map>

class Client;
class Server {

    private:
        int _serverSocket;
        int _portNumber;
        const std::string& _password;
        std::vector<pollfd> fds;
        std::map<int, Client> _clients;
    
    public:
        Server(int port, const  std::string& password);
        ~Server();
        void run();
        void initialize();
        void clientPrintInfo(struct sockaddr_in* clientAddr, int clientSocket);

    private:
        void initSocket();
        void setPollFd();
        //Socket Fonksiyonları:
        void createSocket();
        void configureSocket();
        void bindSocket();
        void setNonBlocking();
        void listenMode();


        void acceptConnection();
        void handleClientMessage(int clientSocket);
        void closeConnection(int clientSocket);

        //Authenticate
        bool authClient(int clientSocket);
        bool verifyPassword(const std::string& password);

        //Message
        void sendMessage(int clientSocket, const std::string& msg);
        std::string receiveMessage(int clientSocket);
};

#endif