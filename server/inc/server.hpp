#pragma once
#include "tsvector.hpp"
#include "tsqueue.hpp"
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
        void messageLoop();
    public:
        typedef struct{
            int fd;
            struct sockaddr_in address;
            socklen_t addressLen;
            int id;
        } client;
    private:
        sc::TSVector<client> clients;
        uint16_t port;
        int socketFD;
        int maxConnections;
        int lastId;

        bool isRunning;
        bool isListening;
        std::mutex isRunningMutex;
        std::mutex isListeningMutex;

        std::thread listenThread;
        std::mutex listenThreadRunning;

        sc::TSQueue<sc::Message> messageQueue;
        std::thread messageThread;
        std::mutex messageThreadRunning;
    };
}
