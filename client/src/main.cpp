#include <cstdio>
#include <iostream>
#include "client.hpp"

int main(int argc,const char* argv[]){
    for (int i = 0; i < argc; i++){
        printf("%s", argv[i]);
    }
    printf("\n");

    char adr[] = "127.0.0.1";
    sc::Client client(adr, 6969);
    client.tryConnect();

    std::cin.get();

    client.tryDisconnect();
    return 0;
}
