#include "../include/Server.hpp"
#include <sstream>

Server::Server(int port, const std::string& password) 
    :_portNumber(port), _password(password) {
    std::cout << "Server Constructer Called!" << std::endl;
}

Server::~Server() {
    std::cout << "Server Destructor Called!" << std::endl;
}

bool Server::authClient(int clientSocket){
    
    sendMessage(clientSocket, "PASS: Enter a password\r\n");

    std::string receivedPass = receiveMessage(clientSocket);
    if(receivedPass.empty()){
        return false;
    }

    if(verifyPassword(receivedPass)){
        _clients[clientSocket].setAuth(true);
        sendMessage(clientSocket, "Welcome to the IRC server\r\n");
        return true;
    } else {
        sendMessage(clientSocket, "Invalid Password. Connection Closed!\r\n");
        return false;
    }
    
    
    
    
}

bool Server::verifyPassword(const std::string& password){
    return _password == password;
}

void Server::sendMessage(int clientSocket, const std::string& msg){
    send(clientSocket, msg.c_str(), msg.length(), 0);
}

std::string Server::receiveMessage(int clientSocket){
    char buffer[1024];
    ssize_t byteRead = recv(clientSocket, buffer, sizeof(buffer) -1, 0);

    if(byteRead <= 0)
        return "";
    buffer[byteRead] = '\0';
    return std::string(buffer);
}

void Server::createSocket(){
    //1. Socket Olusturma
     _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverSocket == -1){
        std::cerr << "Socket Created Failed!" << std::endl;
        return; 
    } 
}

void Server::configureSocket(){
    //2. Socket Ayarlama /SOL_SOCKET = socket seviysei protokol /SO_REUSEADDR = ayni adres ayni port yeniden kullanim izini
    int opt = 1;
    if(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        std::cerr << "Socket Set Failed!" << std::endl;
        close(_serverSocket);
        return;
    }
}

void Server::bindSocket(){
    //3.Soketi adrese ve porta baglama.
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_portNumber);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(_serverSocket, (const sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        std::cerr << "Bind Failed! Maybe port is aldreay use broo" << std::endl;
        exit(1);
        return;
    }
}

void Server::listenMode(){
    //4.Socketi Dinleme Moduna Alma
    if(listen(_serverSocket,10) < 0){
        std::cerr << "Listen Failed" << std::endl;
        close(_serverSocket);
        return;
    }
}

void Server::setNonBlocking(){
    //5.Soketi Bloklanamayan Moda alma
    int flags = fcntl(_serverSocket, F_GETFL, 0);
    flags |= O_NONBLOCK;
    if(fcntl(_serverSocket,F_SETFL,flags) < 0){
        std::cerr << "Non Block Mode Failed" << std::endl;
        return;
    }
}



void Server::initialize(){
    initSocket();
    setPollFd();
}

void Server::clientPrintInfo(struct sockaddr_in* clientAddr, int clientSocket){
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr->sin_addr),ip, INET_ADDRSTRLEN);
    int port = ntohs(clientAddr->sin_port);

    _clients[clientSocket] = Client(clientSocket,std::string(ip), port);
    _clients[clientSocket].setAuth(false);

    pollfd newClient;
    newClient.fd = clientSocket;
    newClient.events = POLLIN;
    fds.push_back(newClient);

    
}

void Server::initSocket(){
    
    createSocket();
    configureSocket();
    bindSocket();
    listenMode();
    setNonBlocking();


    std::cout << "Server Initialized and Listening on Port ===> " << _portNumber << std::endl;
}

void Server::setPollFd(){
    pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;
    //renevt add
    fds.push_back(serverPollFd);
}


void Server::acceptConnection(){


    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
   
    // Yeni Baglanti Kabul Etme
    int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if(clientSocket == -1){
        if(errno != EWOULDBLOCK && errno != EAGAIN){
            std::cerr << "Error Accepting Connection "  << std::endl;
        }
        return;
    }
   
        
        clientPrintInfo(&clientAddr,clientSocket);

        

    std::cout << "New Connection Accepted. Socket: " << clientSocket << std::endl;
}


void Server::handleClientMessage(int clientSocket){
 
    if(!_clients[clientSocket].isAuth()){
        std::string receivedPass = receiveMessage(clientSocket);
        if(receivedPass.empty()){
            closeConnection(clientSocket);
            return;
        }
        if(verifyPassword(receivedPass)){
            _clients[clientSocket].setAuth(true);
            sendMessage(clientSocket, "Welcome to the IRC server\r\n");
        } else {
            sendMessage(clientSocket, "Invalid Password. Connection Closed!\r\n");
            closeConnection(clientSocket);
        }
        return;
    }
    //mesaj işleme
}

void Server::run(){

    SignalHandler::setup();
    while (SignalHandler::isRunning()){
        int ready = poll(fds.data(),fds.size(), -1);
        if(ready == -1){
            if(errno == EINTR) continue;
            std::cerr << "Poll Failed!" << std::endl;
            break;
        }  
        for (size_t i = 0; i < fds.size(); ++i) {
           
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == _serverSocket) {
                    acceptConnection();
                } else {
                    printf("Istemciden Gelen Veriler:\n");
                    handleClientMessage(fds[i].fd);
                } 
            }
        }
    }
    
    std::cout << "Server Shutting Down Succesfully" << std::endl;
}

void Server::closeConnection(int clientSocket){
    close(clientSocket);
    _clients.erase(clientSocket);
    //polfd vektördende kaldırılacak.
    for(size_t i = 0; i < fds.size(); ++i){
        if(fds[i].fd == clientSocket){
            fds.erase(fds.begin() + i);
            break;
        }
    }
}