#include <iostream>


#include"socket_util.hpp"

int main() {
#ifdef Windows
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata));
    {
#endif
        ClientSocketUtil s("127.0.0.1", 9190);

        char x[DATA_MAX_SIZE];
        s.receiveAllMessage(x);
        std::cout<<x;
        s.sendMessage("4/1234");
#ifdef Windows
    }
    WSACleanup();
#endif
    return 0;
}