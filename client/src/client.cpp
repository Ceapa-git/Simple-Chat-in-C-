#include "client.hpp"

namespace sc{
    Client::Client(const char* address, uint16_t port){
        this->address = std::make_unique<char[]>(strlen(address) + 1);
        strcpy(this->address.get(), address);
        this->port = port;
        this->isConnected = false;
        this->id = -1;
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

        Message msg(Message::MessageType::Get);
        unsigned long int len;
        std::unique_ptr<char[]> body;
        msg.readMessage(this->socketFD);
        msg.get(len, body);
        std::istringstream ss(body.get());
        ss >> this->id;

        this->isConnected = true;

        return 0;
    }
    int Client::tryDisconnect(){
        if (!this->isConnected) return -1;
        Message msg(Message::MessageType::Send);
        
        char cmsg[100] = {0};
        sprintf(cmsg, "disconnect");
        std::unique_ptr<char[]> body = std::make_unique<char[]>(strlen(cmsg));
        strncpy(body.get(), cmsg, strlen(cmsg));
        msg.set(strlen(cmsg), body);
        msg.sendMessage(this->socketFD);

        close(this->socketFD);
        return 0;
    }

    void Client::setPort(uint16_t port){
        if(!isConnected) this->port = port;
    }
    uint16_t Client::getPort(){
        return this->port;
    }
    void Client::setAddress(const char* address){
        if(!this->isConnected){
            this->address = std::make_unique<char[]>(strlen(address) + 1);
            strcpy(this->address.get(), address);
        }
    }
    std::unique_ptr<char[]> Client::getAddress(){
        std::unique_ptr<char[]> ret = std::make_unique<char[]>(strlen(this->address.get()) + 1);
        strcpy(ret.get(), this->address.get());
        return ret;
    }
}