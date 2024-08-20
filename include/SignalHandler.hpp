#pragma once

#include <csignal>
#include <iostream>

class SignalHandler {
    
    private:
        static void handler(int signum);
        static sig_atomic_t running; // sig_atomic_T signal handler tarafından okunan ve degiştirebilen tamsayı tipidr. // running sunucunun çalışma durumunu tutar 1 çalışıyor 0 çalışmıyor.

    public:
        static void setup();
        static bool isRunning();
};