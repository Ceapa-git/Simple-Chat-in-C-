#pragma once
#include <cstdint>
#include <memory>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include "message.hpp"

namespace sc{
    class Client{
    public:
        Client(const char* address, uint16_t port);
        int tryConnect();
        int tryDisconnect();

        void setPort(uint16_t port);
        uint16_t getPort();
        void setAddress(const char* address);
        std::unique_ptr<char[]> getAddress();

        int getFD() {return this->socketFD;};
    private:
        uint16_t port;
        std::unique_ptr<char[]> address;
        int socketFD;
        bool isConnected;
        int id;
    };
}
