#include<../include/Client.hpp>

Client::Client(int socket, const std::string& ip, int port){
    _clientSocket = socket;
    _clientİp = ip;
    _clientPort = port;
    std::cout << "CliENT Oluşturuldu: " << "Socket: "<< socket << "ip: " << ip <<" port: " <<port << std::endl; 
}
Client::~Client(){}

Client::Client(){}