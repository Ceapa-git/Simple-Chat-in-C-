#pragma once
#include <unistd.h>
#include <sys/socket.h>
#include <memory>
#include <cstring>
#include <cstdlib>

namespace sc{
    class Message{
    public:
        enum class MessageType {Send,Get};
        Message() = delete;
        Message(MessageType messageType);

        int get(long unsigned int &size, std::unique_ptr<char[]> &message);
        int set(long unsigned int size, std::unique_ptr<char[]> const &message);
        int readMessage(int fd);
        int sendMessage(int fd);
    private:
        bool ready;
        MessageType messageType;
        struct Header{
            size_t messageSize;
            size_t bodySize;
        }header;
        static const size_t headerSize = sizeof(Header);
        std::unique_ptr<char[]> body;
    };
}