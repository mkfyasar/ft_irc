#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>

class Server {

    private:
        int _port;
        std::string _password;
        int _serverSocket;
        std::vector<int> _clientSockets;
        std::map<int, std::string> _clientNames;
    
    public:
        Server(int port, const  std::string& password);
        ~Server();

        void start();
        void stop();
    
    private:
        void setupServerSocket();

};

#endif