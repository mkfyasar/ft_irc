#include<../include/Client.hpp>

Client::Client(){}

Client::Client(int socket, const std::string& ip, int port){
    _clientSocketFd = socket;
    _clientİp = ip;
    _clientPort = port;
    _isAuth = false;
}
Client::~Client(){}
bool Client::isAuth() const {return _isAuth;}

//get
int Client::getFd() const {return _clientSocketFd;}
const std::string& Client::getNickname () const {return _nickname;}
const std::string& Client::getUsername () const {return _username;}

//set
void Client::setAuth(bool auth) {_isAuth = auth;}
void Client::setNickname(const std::string& nickname){_nickname = nickname;}
void Client::setUsername(const std::string& username){_username = username;}

//Authenticat
void Client::authenticate(){_isAuth = true;}
void Client::deauthenticate(){_isAuth = false;}
