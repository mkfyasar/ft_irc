#pragma once

#include <iostream>
#include <string>

class Client{

    private:
        int _clientSocket;
        std::string _clientİp;
        int _clientPort;

    public:
        Client();
        Client(int socket, const std::string& ip, int port);
        ~Client();


};