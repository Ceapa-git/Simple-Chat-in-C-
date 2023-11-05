#pragma once
#include <cstdint>
#include <vector>

namespace sc{
    class Server{
    public:
        Server(uint16_t port, int maxConnections);
        int tryStart();
        int tryStop();

        void setPort(uint16_t port);
        uint16_t getPort();
        void setMaxConnections(int maxConnections);
        int getMaxConnections();
    private:
        uint16_t port;
        int socketFD;
        int maxConnections;
        bool isRunning;
        std::vector<int> clients;
    };
}
