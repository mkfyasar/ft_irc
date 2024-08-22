#include "../include/Server.hpp"
#include <sstream>

Server::Server(int port, const std::string& password) 
    :_portNumber(port), _password(password) {
}

Server::~Server() {}

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
    std::cout << _password << " ==== " << password << std::endl;
    return _password == password;
}

bool Server::sendMessage(int clientSocket, const std::string& msg) {
    size_t totalSend = 0;
    size_t remaining = msg.length();

    while (totalSend < msg.length()) {
        ssize_t sent = send(clientSocket, msg.c_str() + totalSend, remaining, 0);
        if (sent < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                // daha sonra tekrar dene
                continue;
            } else {
                throw ServerException("Error sending message to client");
            }
        } else if (sent == 0) {
            // baglanti kaoandi
            return false;
        }
        totalSend += sent;
        remaining -+ sent;
    }
    return true;
}

std::string Server::receiveMessage(int clientSocket){
    char buffer[BUFFER_SIZE];
    std::string message;
    ssize_t bytesRead ;

    while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        message += buffer;
        if (message.find("\r\n") != std::string::npos) {
            break;
        }
    }

    if(bytesRead == 0){
        //Baglanti kapandi
        std::cout << RED << "Client <" << clientSocket - 3 << "> Disconnected" << WHITE << std::endl;
        closeConnection(clientSocket);
        return "";
    } else if (bytesRead < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
        throw ServerException("Error receiving message from client");
    }
    return message;
}

void Server::createSocket(){
    //1. Socket Olusturma
     _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverSocket == -1){
        throw ServerException("Failed to create socket");
    } 
}

void Server::configureSocket(){
    //2. Socket Ayarlama /SOL_SOCKET = socket seviysei protokol /SO_REUSEADDR = ayni adres ayni port yeniden kullanim izini
    int opt = 1;
    if(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        throw ServerException("Failed to configure socket");
    }
}

void Server::bindSocket(){
    //3.Soketi adrese ve porta baglama.
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_portNumber);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(_serverSocket, (const sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        throw ServerException("Failed bind mode maybe port already used");
        exit(1);
    }
}

void Server::listenMode(){
    //4.Socketi Dinleme Moduna Alma
    if(listen(_serverSocket,10) < 0){
        throw ServerException("Failed to listen on socket");
    }
}

void Server::setNonBlocking(int socket){
    //5.Soketi Bloklanamayan Moda alma
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1) {
        throw ServerException("Failed to get server socket flags");
    }
    flags |= O_NONBLOCK;
    std::cout<< YELLOW << socket  << ": ===> Set NonBlocking Mode" << WHITE << std::endl;
    if(fcntl(socket,F_SETFL,flags) < 0)
        throw ServerException("Failed to set NonBlocking mode server socket");
}


void Server::initialize(){
    setupServerSocket();
    setPollFd();
}

void Server::addNewClient(struct sockaddr_in* clientAddr, int clientSocket){
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr->sin_addr),ip, INET_ADDRSTRLEN);
    int port = ntohs(clientAddr->sin_port);

    _clients[clientSocket] = Client(clientSocket,std::string(ip), port);

    std::cout << GREEN << "Client <" << clientSocket - 3 << "> Connected from " << ip << ":" << port << WHITE << std::endl;
}

void Server::setupServerSocket(){
    
    try {
        createSocket();
        configureSocket();
        bindSocket();
        listenMode();
        setNonBlocking(_serverSocket);
    } catch (const ServerException& e){
        std::cerr << "Socket initialization failed: " << e.what() << std::endl;
        exit(1);
    }
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
            throw ServerException("Failed aceppting connection");
        }
        return;
    }
        setNonBlocking(clientSocket);
        _pollManager.addFd(clientSocket, POLL_IN);

        addNewClient(&clientAddr, clientSocket);
    std::cout << GREEN << "Client <" << clientSocket -3 << "> Connected" << WHITE << std::endl;
}


void Server::handleClientMessage(int clientSocket){
    try {
        std::string message = receiveMessage(clientSocket);
        if (message.empty()) {
            return; // baglanti zaten kapatildi
        }

        Client& client = _clients[clientSocket];
        //Mesajisleme burada oalcak messagehandler

        //gecici deneme kaldirilacak
        if(!sendMessage(clientSocket, "Server received: " + message)) {
            closeConnection(clientSocket);
        }
    } catch (const ServerException& e) {
        std::cerr << "Error handling client message: " << e.what() << std::endl;
        closeConnection(clientSocket);
    }
}

void Server::run(){

    SignalHandler::setup();
    std::cout << GREEN << "Server <" << _serverSocket - 2 << "> Connected" << WHITE << std::endl;
    std::cout << "Waiting to accept a connection..." << std::endl;

    _pollManager.addFd(_serverSocket, POLL_IN);

    while (SignalHandler::isRunning()){
        int ready = _pollManager.poll(-1);
        if(ready == -1){
            if(errno == EINTR) continue;
            throw ServerException("Poll Failed");
        }

        const std::vector<pollfd>& fds = _pollManager.getFds();  
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == _serverSocket) {
                    acceptConnection();
                } else {
                    handleClientMessage(fds[i].fd);
                } 
            }
        }
    }
    
    std::cout << "Server Shutting Down Succesfully" << std::endl;
}


void Server::closeConnection(int clientSocket){
    try {
        //soket kapama
        if (close(clientSocket) < 0) {
            throw ServerException("Error closing client socket");
        }
        //poll kaldirma
        _pollManager.removeFd(clientSocket);

        //client map kaldirma
        std::map<int, Client>::iterator it = _clients.find(clientSocket);
        if(it != _clients.end()) {
            //burda chanel close eklenicek
            _clients.erase(it);
        }
        std::cout << YELLOW << "Client <" << clientSocket - 3 << "> Connection closed" << WHITE << std::endl;
    } catch (const ServerException& e) {
        std::cerr << "Error in closeConnection: " << e.what() << std::endl;
    }
}