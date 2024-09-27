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
        std::cout << x<<'\n';
        socketNotes t;
        t.pushElement(s.clnt_sock_);
        t.setNotes("no.1");

        ClientSocketUtil ss("127.0.0.1",9190);
        s.sendMessage("4/1234");
        SocketUtil::receiveAllMessage(ss.clnt_sock_, x);
        std::cout << x;
        t.pushElement(ss.clnt_sock_);
        t.setNotes("no.2");
        auto xw=t.find("no.1");
        SocketUtil::sendMessage((*xw).first,"8/I'm no.1");
    }
    WSACleanup();
    return 0;
}
