#include <iostream>


#include"socket_util.h"

int main() {
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata));
    ServerSocketUtil s(9190);
    s.acceptSocket();
    char x[123];
    int len=s.receiveAllMessage(x);
    x[len]=0;
    std::cout<<x;
    WSACleanup();
    return 0;
}
