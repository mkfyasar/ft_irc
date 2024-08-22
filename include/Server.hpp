#ifndef SERVER_HPP
#define SERVER_HPP



#define GREEN "\e[1;32m"
#define RED "\e[1;31m"
#define WHITE "\e[1;37m"
#define YELLOW "\e[1;33m"

#include "../include/SignalHandler.hpp"
#include "../include/Client.hpp"
#include "../include/ServerException.hpp"
#include "../include/PollManager.hpp"

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
        PollManager _pollManager;
        static const size_t BUFFER_SIZE = 1024;
    
    public:
        Server(int port, const  std::string& password);
        ~Server();
        void run();
        void initialize();
        void addNewClient(struct sockaddr_in* clientAddr, int clientSocket);

    private:
        void setPollFd();
        //Socket Fonksiyonları:
        void setupServerSocket();
        void setNonBlocking(int socket);
        void createSocket();
        void configureSocket();
        void bindSocket();
        void listenMode();


        void acceptConnection();
        void handleClientMessage(int clientSocket);
        void closeConnection(int clientSocket);

        //Authenticate
        bool authClient(int clientSocket);
        bool verifyPassword(const std::string& password);

        //Message
        bool sendMessage(int clientSocket, const std::string& msg);
        std::string receiveMessage(int clientSocket);
};

#endif