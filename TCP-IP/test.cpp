#include <iostream>


#include"socket_util.h"

int main() {
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata));
    {
        ClientSocketUtil s("127.0.0.1", 9190);
        s.sendMessage("4/1234");
        char x[DATA_MAX_SIZE];
        SocketUtil::receiveAllMessage(s.clnt_sock_, x);
        std::cout << x;
    }
    WSACleanup();
    return 0;
}
