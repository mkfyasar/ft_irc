#pragma once

#include <iostream>
#include <string>

class Client{

    private:
        int _clientSocketFd;
        std::string _clientİp;
        int _clientPort;
        std::string _nickname;
        std::string _username;
        bool _isAuth;

    public:
        Client();
        ~Client();
        Client(int socket, const std::string& ip, int port);

        //get
        int getFd() const;
        const std::string& getNickname() const;
        const std::string& getUsername() const;

        //set
        void setNickname(const std::string& nickname);
        void setUsername(const std::string& username);
        void setAuth(bool auth);

        //Authenticate
        bool isAuth() const;
        void authenticate();
        void deauthenticate();

};