#include "server.hpp"

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

        if ((this->socketFD = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1){
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

        return 0;
    }
    int Server::tryStop(){
        if (!this->isRunning) return -1;

        std::unique_lock<std::mutex> isListeningLock(this->isListeningMutex);
        this->isListening = false;
        isListeningLock.unlock();
        std::unique_lock<std::mutex> isRunningLock(this->isRunningMutex);
        this->isRunning = false;
        isRunningLock.unlock();

        std::lock_guard<std::mutex> lock(this->threadRunning);
        this->listenThread.join();
        close(this->socketFD);
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
        std::lock_guard<std::mutex> lock(this->threadRunning);
        std::unique_lock<std::mutex> isRunningLock(this->isRunningMutex);
        std::unique_lock<std::mutex> isListeningLock(this->isListeningMutex);
        isRunningLock.unlock();
        isListeningLock.unlock();
        while(true){
            isRunningLock.lock();
            if(!this->isRunning){
                isRunningLock.unlock();
                break;
            }
            isRunningLock.unlock();

            while(true){
                isListeningLock.lock();
                if(!this->isListening){
                    isListeningLock.unlock();
                    break;
                }

                isListeningLock.unlock();
                struct sockaddr_in clientAddress;
                socklen_t clientAddressLen = sizeof(clientAddress);
                int clientFD = accept(this->socketFD, (struct sockaddr*)&clientAddress, &clientAddressLen);
                if (clientFD == -1){
                    if (errno != EAGAIN || errno != EWOULDBLOCK){
                        perror("accept");
                    }
                }
                else{
                    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&clientAddress;
                    struct in_addr ipAddr = pV4Addr->sin_addr;
                    char str[INET_ADDRSTRLEN];
                    inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN);

                    Message msg(Message::MessageType::Send);
                    char cmsg[100] = {0};
                    sprintf(cmsg, "%s connected", str);
                    std::unique_ptr<char[]> body = std::make_unique<char[]>(strlen(cmsg));
                    strncpy(body.get(), cmsg, strlen(cmsg));
                    msg.set(strlen(cmsg), body);

                    printf("%s\n", cmsg);
                    
                    for(long unsigned int i = 0; i < this->clients.size(); i++){
                        if(this->clients.notEmpty(i)){
                            msg.sendMessage(this->clients[i].fd);
                        }
                    }
                    this->clients.insert({clientFD, clientAddress, clientAddressLen});
                    //todo callback
                }
            }
        }
    }
}