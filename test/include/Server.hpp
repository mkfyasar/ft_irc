#pragma once


#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <errno.h>

class Server{
    private:
        int _socketFd;
        int _portNumber;
        const std::string& _password;
        std::string result;

    public:
        Server(int portNumber, const std::string& password);
        ~Server();
        int createSocket();
        int createBind(int socketFd, int portNumber);
        int createListen(int socketFd);
        int createAccept(int socketFd);
        int createRecv(int socketFd);
        int setSocket(int socketFd);
        int getPortNumber();
        int getSocketFd();
        int setNonBlocking(int socketFd);
};