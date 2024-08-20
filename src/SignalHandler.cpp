#include "../include/SignalHandler.hpp"


sig_atomic_t SignalHandler::running = 1;

void SignalHandler::setup(){
    std::signal(SIGINT, SignalHandler::handler); // ctrl c , sinyal alınınca cagrılaak fonksiyon
    std::signal(SIGTERM, SignalHandler::handler);
    std::signal(SIGQUIT,SignalHandler::handler);
}

void SignalHandler::handler(int signum){ // handler alınan sinyal numarasını içerir. sinyal alınınca bu fonksiyon çağrılır.
    
    
    running = 0; // sunucu kapatmaya alır.
    std::cout << "Received Signal " << signum << ".Shutting Down..." << std::endl;
}

bool SignalHandler::isRunning(){ // sunucunun hala çalışıp çalışmadıgını kontrol eder.
    return running;
}
