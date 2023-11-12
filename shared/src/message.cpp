#include "message.hpp"
#include <cstdio>

namespace sc{
    Message::Message(MessageType messageType){
        this->ready = false;
        this->messageType = messageType;
    }

    int Message::get(long unsigned int &size, std::unique_ptr<char[]> &message){
        if(this->messageType == MessageType::Send || !this->ready)
            return -1;
        
        size = this->header.bodySize;
        message.reset();
        message = std::make_unique<char[]>(size);
        strncpy(message.get(), this->body.get(), size);
        
        return 0;
    }
    int Message::set(long unsigned int size, std::unique_ptr<char[]> const &message){
        if(this->messageType == MessageType::Get || this->ready)
            return -1;
        
        this->header.bodySize = size;
        this->header.messageSize = this->header.bodySize + this->headerSize;
        this->body = std::make_unique<char[]>(size);
        strncpy(this->body.get(), message.get(), size);

        this->ready = true;
        return 0;
    }
    int Message::readMessage(int fd){
        if(this->messageType == MessageType::Send || this->ready)
            return -1;
        
        char* buff = (char*) malloc(this->headerSize);
        ssize_t amountRead = read(fd, buff, this->headerSize);
        if(amountRead == -1 || (size_t)amountRead != this->headerSize){
            free(buff);
            return -2;
        }
        this->header.messageSize = *(size_t*)buff;
        this->header.bodySize = *(size_t*)(buff + sizeof(size_t));
        free(buff);

        buff = (char*) malloc(this->header.bodySize);
        amountRead = read(fd, buff, this->header.bodySize);
        if(amountRead == -1 || (size_t)amountRead != this->header.bodySize){
            free(buff);
            return -3;
        }
        this->body = std::make_unique<char[]>(this->header.bodySize);
        strncpy(this->body.get(), buff, this->header.bodySize);
        free(buff);

        this->ready = true;
        return 0;
    }
    int Message::sendMessage(int fd){
        if(this->messageType == MessageType::Get || !this->ready)
            return -1;
        
        char* buff = (char*) malloc(this->header.messageSize);
        strncpy(buff, (char*)(&(this->header.messageSize)), sizeof(size_t));
        strncpy(buff + sizeof(size_t), (char*)(&(this->header.bodySize)), sizeof(size_t));
        strncpy(buff + this->headerSize, this->body.get(), this->header.bodySize);
        ssize_t amountSent = send(fd, buff, this->header.messageSize, 0);
        if(amountSent == -1 || (size_t)amountSent != this->header.messageSize){
            free(buff);
            return -2;
        }
        free(buff);

        return 0;
    }
}