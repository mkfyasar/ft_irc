#ifndef SERVER_HPP
#define SERVER_HPP

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

class Server {

    private:
        int _serverSocket;
        int _portNumber;
        const std::string& _password;
        std::vector<pollfd> fds;
    
    public:
        Server(int port, const  std::string& password);
        ~Server();
        void run();

    
    private:
        void initSocket();
        void setPollFd();
        void acceptConnection();
        void handleClientMessage(int clientSocket);
};

#endif