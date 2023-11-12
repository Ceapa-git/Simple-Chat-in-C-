#include <cstdio>
#include <iostream>
#include "client.hpp"
#include "message.hpp"
#include <memory>

int main(int argc,const char* argv[]){
    for (int i = 0; i < argc; i++){
        printf("%s", argv[i]);
    }
    printf("\n");

    char adr[] = "127.0.0.1";
    sc::Client client(adr, 6969);
    client.tryConnect();

    while(true){
        sc::Message msg(sc::Message::MessageType::Get);
        msg.readMessage(client.getFD());
        long unsigned int size;
        std::unique_ptr<char[]> cmsg;
        int getVal = msg.get(size, cmsg);
        if(getVal == 0)
            printf("%.*s\n", (int)size, cmsg.get());
        else
            printf("%d\r", getVal);
    }

    client.tryDisconnect();
    return 0;
}
