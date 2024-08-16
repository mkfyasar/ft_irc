#include "../include/Server.hpp"

Server::Server(int portNumber, const std::string& password):_portNumber(portNumber),_password(password){
    std::cout << "Port Number : " << _portNumber << " Password : " << _password << std::endl;
}
Server::~Server(){
    std::cout << "Server Destructor" << std::endl;
}



int Server::createSocket(){

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd == -1)
        std::cout << "Socket Created Failed!" << std::endl;
    printf("Socket Olusturuldu\n");
    return socketFd;
}

int Server::createBind(int socketFd, int portNumber){

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNumber);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bindFd = bind(socketFd, (struct sockaddr*)&server_addr,sizeof(server_addr));
    if(bindFd == -1)
        std::cout << "Bind Created Failed!" << std::endl;
    else
        std::cout << "Bind Calisti Socket Dinleme Moduna Alindi." << std::endl;

    return bindFd;
}

int Server::createListen(int socketFd){
    int listFd = listen(socketFd, 5);
    if(listFd == -1)
        std::cout << "Bind Created Failed!" << std::endl;
    else
        std::cout << "Listen Fonksiyonu Socket icin ayarlandi. " << std::endl;
    return listFd;
}

int Server::createAccept(int socketFd){
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int clientFd = accept(socketFd, (struct sockaddr *)&client_addr,&client_len);
    
    return clientFd;
}

int Server::createRecv(int socketFd){
    char buffer[1024];
    ssize_t bytes_received = recv(socketFd, buffer, sizeof(buffer)-1 , 0);
    
    while (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
        result += buffer;
    }
    return bytes_received;
}

int Server::getPortNumber(){
    return _portNumber;
}
int Server::getSocketFd(){
    return _socketFd;
}


int Server::setSocket(int socketFd){
    int opt = 1;
    int opFd = setsockopt(socketFd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt,  sizeof(opt));
    if(opFd != 0){
        printf("Socket Ayarlanamadi!!!\n");
    }else{
        printf("Socket Secenekleri Basariyla Ayarlandi\n");
    }
    return opFd;
}

int Server::setNonBlocking(int socketFd){
    int flags = fcntl(socketFd,F_GETFL , 0);

    return fcntl(socketFd,F_SETFL, flags | O_NONBLOCK);
}