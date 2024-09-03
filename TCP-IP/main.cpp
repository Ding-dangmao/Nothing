#include <iostream>
#include<functional>

#include"socket_util.h"


void said(std::string x){
    std::cout<<x<<std::endl;
}


int main() {
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata));
    ServerSocketUtil s(9190);
    IOServerSocketUtil w(s);
    w.simpleIOMultiplex(said);

    WSACleanup();
    return 0;
}
