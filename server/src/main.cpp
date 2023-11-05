#include <cstdio>
#include <iostream>
#include "server.hpp"

int main(int argc,const char* argv[]){
    for (int i = 0; i < argc; i++){
        printf("%s", argv[i]);
    }
    printf("\n");

    sc::Server server(6969, 3);
    server.tryStart();

    std::cin.get();

    server.tryStop();
    return 0;
}
