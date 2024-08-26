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
    //SocketUtil()=default;
    SocketUtil(std::string ip,unsigned int port);
    SocketUtil(const SocketUtil&) = delete;
    SocketUtil& operator=(const SocketUtil&) = delete;
    ~SocketUtil()=default;

    //功能函数
    //不维护现套接字的数据传输
    int sendMessage(const std::string& message)const;
    int receiveMessage(char* message)const;
    int receiveMessage(char* message,unsigned int lenn,unsigned int left)const;
    int receiveAllMessage(char* message);
    //指定套接字的数据传输
    static int sendMessage(const SOCKET& sock,const std::string& message);
    static int receiveMessage(const SOCKET& sock, char* message);
    static int receiveMessage(const SOCKET& sock, char* message,unsigned int lenn, unsigned int left);
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
    SOCKET acceptSocket();
    ~ServerSocketUtil();
};

/*此类仅用于数据的传输(public继承于SocketUtil)*/
class ClientSocketUtil:public SocketUtil{
public:
    ClientSocketUtil()=delete;
    explicit ClientSocketUtil(std::string ip,unsigned int port);
    ~ClientSocketUtil();
};
#endif //LEI_NET_SOCKET_UTIL_H
