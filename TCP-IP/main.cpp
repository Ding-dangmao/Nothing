#include <iostream>
#include<functional>

#include"socket_util.hpp"


void said(std::string x){
    std::cout<<x<<std::endl;
}


int main() {
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata));
    {
        ServerSocketUtil s(9190);
        s.acceptSocket();
        char x[12213];
        s.receiveAllMessage(x);
        std::cout<<x;
    }
    WSACleanup();
    return 0;
}
