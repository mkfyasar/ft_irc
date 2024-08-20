#pragma once

#include <iostream>
#include <string>

class Client{

    private:
        int _clientSocket;
        std::string _clientİp;
        int _clientPort;
        bool _isAuth;

    public:
        Client();
        Client(int socket, const std::string& ip, int port);
        ~Client();
        bool isAuth() const;
        void setAuth(bool auth);


};