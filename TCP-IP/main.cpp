#include <iostream>
#include<functional>

#include"socket_util.hpp"


void said(std::string x){
    std::cout<<x<<std::endl;
}


int main() {
#ifdef Windows
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata));
    {
#endif
        ServerSocketUtil s(9190);
        //IOServerSocketUtil i(s);
        //i.argumentSet(5,5000);
        //i.simpleIOMultiplex(said);
        AsyncNotifyIO i(s);
        i(said);
        //IOMultiplexEPOLL d(s,2);
        //d(said);

#ifdef Windows
    }
    WSACleanup();
#endif
    return 0;
}
