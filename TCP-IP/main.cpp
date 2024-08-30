#include <iostream>


#include"socket_util.h"

int main() {
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata));
    ServerSocketUtil s(9190);
    IOServerSocketUtil w(s);
    w.simpleIOMultiplex();

    WSACleanup();
    return 0;
}
