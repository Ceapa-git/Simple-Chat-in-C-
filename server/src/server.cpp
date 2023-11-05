#include "server.hpp"
#include "tsvector.hpp"
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace sc{
    Server::Server(uint16_t port, int maxConnecitons){
        this->port = port;
        this->maxConnections = maxConnecitons;
        this->isRunning = false;
        this->isListening = false;
    }
    int Server::tryStart(){
        if (this->isRunning) return -2;
        if (this->isListening) return -3;

        if ((this->socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            perror("socket");
            return -1;
        }

        int opt = 1;
        if (setsockopt(this->socketFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1){
            perror("setsockopt");
            return -1;
        }
        
        struct sockaddr_in socketAddress;
        socklen_t socketAddressLen = sizeof(socketAddress);
        socketAddress.sin_family = AF_INET;
        socketAddress.sin_addr.s_addr = INADDR_ANY;
        socketAddress.sin_port = htons(this->port);
        if (bind(this->socketFD, (struct sockaddr*)&socketAddress, socketAddressLen) == -1){
            perror("bind");
            return -1;
        }

        if (listen(this->socketFD, this->maxConnections) == -1){
            perror("listen");
            return -1;
        }

        this->isListening = true;
        this->isRunning = true;

        this->listenThread = std::thread(&Server::listenLoop, this);
        this->listenThread.detach();

        return 0;
    }
    int Server::tryStop(){
        if (!this->isRunning) return -1;
        this->isListening = false;
        this->isRunning = false;
        close(this->socketFD);
        //this->listenThread.join();
        return 0;
    }

    void Server::setPort(uint16_t port){
        if(!this->isRunning) this->port = port;
    }
    uint16_t Server::getPort(){
        return this->port;
    }
    void Server::setMaxConnections(int maxConnections){
        if(!this->isRunning) this->maxConnections = maxConnections;
    }
    int Server::getMaxConnections(){
        return this->maxConnections;
    }

    void Server::listenLoop(){
        while(this->isRunning){
            while(this->isListening){
                struct sockaddr_in clientAddress;
                socklen_t clientAddressLen = sizeof(clientAddress);
                int clientFD = accept(this->socketFD, (struct sockaddr*)&clientAddress, &clientAddressLen);
                if (clientFD == -1){
                    perror("accept");
                }
                else{
                    this->clients.insert({clientFD, clientAddress, clientAddressLen});
                    //todo callback
                }
            }
        }
        printf("leave loop\n");
    }
}