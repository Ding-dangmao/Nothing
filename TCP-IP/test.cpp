#include <iostream>


#include"socket_util.hpp"

int main() {
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata));
    {
        ClientSocketUtil s("127.0.0.1", 9190);
        s.sendMessage("4/1234");
    }
    WSACleanup();
    return 0;
}
