#pragma once
#include "tsvector.hpp"
#include "message.hpp"
#include <cstdint>
#include <cstdio>
#include <memory>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace sc{
    class Server{
    public:
        Server(uint16_t port, int maxConnections);
        int tryStart();
        int tryStop();
        /*TODO
        int tryStartListening();
        int tryStopListening();
        */

        void setPort(uint16_t port);
        uint16_t getPort();
        void setMaxConnections(int maxConnections);
        int getMaxConnections();
    private:
        void listenLoop();
    public:
        typedef struct{
            int fd;
            struct sockaddr_in address;
            socklen_t addressLen;
        } client;
    private:
        uint16_t port;
        int socketFD;
        int maxConnections;
        bool isRunning;
        bool isListening;
        sc::TSVector<client> clients;
        std::thread listenThread;
        std::mutex threadRunning;
        std::mutex isListeningMutex;
        std::mutex isRunningMutex;
    };
}
