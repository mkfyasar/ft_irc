#pragma once

#include <vector>
#include <poll.h>

class PollManager {

    private:
        std::vector<pollfd> _fds;
    
    public:
        void addFd(int fd, short events);
        void removeFd(int fd);
        int poll(int timeout);
        const std::vector<pollfd>& getFds() const;
};