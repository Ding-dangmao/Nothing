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
        IOServerSocketUtil i(s);
        i.argumentSet(5,5000);
        i.simpleIOMultiplex(said);
    }
    WSACleanup();
    return 0;
}
