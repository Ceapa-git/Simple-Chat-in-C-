#include "client.hpp"
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace sc{
    Client::Client(std::unique_ptr<char> address, uint16_t port){
        this->address = std::make_unique<char>(address);
        this->port = port;
        this->isConnected = false;
    }
    int Client::tryConnect(){
        if (this->isConnected) return -2;

        if ((this->socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            perror("socket");
            return -1;
        }
        
        struct sockaddr_in socketAddress;
        socklen_t socketAddressLen = sizeof(socketAddress);
        socketAddress.sin_family = AF_INET;
        socketAddress.sin_port = htons(this->port);
        if (inet_pton(AF_INET, this->address.get(), &socketAddress.sin_addr) == -1){
            perror("inet_pton");
            return -1;
        }

        if (connect(this->socketFD, (struct sockaddr*)&socketAddress, socketAddressLen) == -1){
            perror("connect");
            return -1;
        }

        return 0;
    }
    int Client::tryDisconnect(){
        if (!this->isConnected) return -1;
        close(this->socketFD);
        return 0;
    }

    void Client::setPort(uint16_t port){
        if(!isConnected) this->port = port;
    }
    uint16_t Client::getPort(){
        return this->port;
    }
    void Client::setAddress(std::unique_ptr<char> address){
        if(!this->isConnected) this->address = std::make_unique<char>(address);
    }
    std::unique_ptr<char> Client::getAddress(){
        return std::make_unique<char>(this->address);
    }
}