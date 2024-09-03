#ifndef LEI_NET_SOCKET_UTIL_H
#define LEI_NET_SOCKET_UTIL_H

#include<iostream>

#include<winsock2.h>
#include<cstdlib>

#include"errors.h"

#define DATA_MAX_SIZE 2048
#define TINY_CHAR 16


/*此类仅用于数据的传输,不对数据做任何处理(基类)*/
class SocketUtil {
public:
    //构造析构
    SocketUtil()=default;
    SocketUtil(std::string ip,unsigned int port);
    SocketUtil(const SocketUtil&);
    SocketUtil& operator=(const SocketUtil&) = delete;
    ~SocketUtil()=default;

    //功能函数
    //不维护现套接字的数据传输
    int sendMessage(const std::string& message)const;
    int receiveMessage(char* message)const;
    int receiveMessage(char* message,int lenn,unsigned int left)const;
    int receiveAllMessage(char* message)const;
    //指定套接字的数据传输
    static int sendMessage(const SOCKET& sock,const std::string& message);
    static int receiveMessage(const SOCKET& sock, char* message);
    static int receiveMessage(const SOCKET& sock, char* message,int lenn, unsigned int left);
    static int receiveAllMessage(const SOCKET& sock,char *message);

    SOCKET server_sock_;
    SOCKET clnt_sock_;
    SOCKADDR_IN server_addr_;
    SOCKADDR_IN client_addr_;
    int client_addr_sz;

    std::string ip_;
    unsigned int port_;
};

/*此类仅用于数据的传输(public继承于SocketUtil)*/
class ServerSocketUtil:public SocketUtil{
public:
    ServerSocketUtil()=delete;
    explicit ServerSocketUtil(unsigned int port);

    ServerSocketUtil(const ServerSocketUtil&);

    SOCKET acceptSocket();
    ~ServerSocketUtil();
};

/*此类仅用于数据的传输(public继承于SocketUtil)*/
class ClientSocketUtil:public SocketUtil{
public:
    ClientSocketUtil()=delete;
    explicit ClientSocketUtil(const std::string& ip,unsigned int port);
    ~ClientSocketUtil();
};

class IOServerSocketUtil{
public:
    struct MemberStructure {
        MemberStructure():fd_nums(0){}
        fd_set fd_set_;
        fd_set fd_set_cpy;
        timeval timeout;
        long timeout_seconds,timeout_microseconds;
        int fd_nums;
    };
public:
    IOServerSocketUtil()=delete;
    inline explicit IOServerSocketUtil(ServerSocketUtil& server): member_structure_(),server_socket_util_(server){
        FD_ZERO(&member_structure_.fd_set_);
        FD_SET(server.server_sock_,&member_structure_.fd_set_);
        ++member_structure_.fd_nums;
    }
private:
    void addFD(const SOCKET& sock);
    void deleteFD(const SOCKET& sock);
    void argumentSet(long timeout_seconds=5,long timeout_microseconds=5000);
public:
    template<class T>
    void simpleIOMultiplex(T);

    ServerSocketUtil server_socket_util_;
    MemberStructure member_structure_;
};


#endif //LEI_NET_SOCKET_UTIL_H


template<typename T>
void IOServerSocketUtil::simpleIOMultiplex(T function){
    int return_val;
    int len;
    while(true) {
        member_structure_.fd_set_cpy=member_structure_.fd_set_;
        member_structure_.timeout.tv_sec = member_structure_.timeout_seconds;
        member_structure_.timeout.tv_usec = member_structure_.timeout_microseconds;
        fd_set& fdset=member_structure_.fd_set_cpy;
        if((return_val= select(member_structure_.fd_nums,&fdset,
                               0,0,&member_structure_.timeout))==-1){
            lei_net_error::throwException("select error!",2);
            break;
        }
        if(return_val==0){std::puts("timeout");continue;}

        for(int i=0;i<fdset.fd_count;++i){
            if(FD_ISSET(fdset.fd_array[i],&fdset)){
                SOCKET sock=fdset.fd_array[i];
                if(sock==server_socket_util_.server_sock_){
                    SOCKET clnt = server_socket_util_.clnt_sock_=server_socket_util_.acceptSocket();
                    addFD(clnt);
                    SocketUtil::sendMessage(clnt,"20/receive your message");
                    std::puts("something in");
                }
                else{
                    char message[DATA_MAX_SIZE];
                    len = SocketUtil::receiveAllMessage(sock,message);
                    if(len==0){
                        deleteFD(sock);
                        std::puts("close sock");
                        closesocket(sock);
                    }else{
                        //函数调用
                        T(message);
                        //std::cout<<message<<'\n';
                    }
                }
            }
        }
    }
}