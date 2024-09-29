#ifndef LEI_NET_SOCKET_UTIL_H
#define LEI_NET_SOCKET_UTIL_H

#include<iostream>
#include<algorithm>
#include<vector>


#include<cstdlib>

#include"errors.h"

#ifdef Windows
#include<winsock2.h>
using socket_N=SOCKET;
using sockaddr_in_N = SOCKADDR_IN;
using sockaddr_N = SOCKADDR;
using socklen_t_N = int;

#define SOCKET_ERROR_N SOCKET_ERROR
#define INVALID_SOCKET_N INVALID_SOCKET

#define SEND_MESSAGE(sock_N,string_N,len_N,flags_N) send(sock_N,string_N.c_str(),len_N,flags_N)
#define RECEIVE_MESSAGE(sock_N,string_N,len_N,flags_N) recv(sock_N,string_N,len_N,flags_N)

#endif

#ifdef Linux
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>

using socket_N=int;
using sockaddr_in_N = struct sockaddr_in;
using sockaddr_N = struct sockaddr;
using socklen_t_N = socklen_t;

#define SOCKET_ERROR_N -1
#define INVALID_SOCKET_N -1

#define SEND_MESSAGE(sock_N,string_N,len_N,flags_N) write(sock_N,string_N.c_str(),len_N)
#define RECEIVE_MESSAGE(sock_N,string_N,len_N,flags_N) read(sock_N,string_N,len_N)

#endif


#define DATA_MAX_SIZE 2048
#define TINY_CHAR 16

class socketNotes;
class ServerSocketUtil;
class ClientSocketUtil;
/*此类仅用于数据的传输,不对数据做任何处理(基类)*/
class SocketUtil {
    friend ServerSocketUtil;
    friend ClientSocketUtil;
public:
    //构造析构
    SocketUtil()=default;
    SocketUtil(std::string ip,unsigned int port);
    SocketUtil(const SocketUtil&)=default;
    SocketUtil& operator=(const SocketUtil&) = delete;
    ~SocketUtil()=default;

    //功能函数
    //不维护现套接字的数据传输
    int sendMessage(const std::string& message)const;
    int receiveMessage(char* message)const;
    int receiveMessage(char* message,int lenn,unsigned int left)const;
    int receiveAllMessage(char* message)const;
    //指定套接字的数据传输
    static int sendMessage(const socket_N& sock,const std::string& message);
    static int receiveMessage(const socket_N & sock, char* message);
    static int receiveMessage(const socket_N& sock, char* message,int lenn, unsigned int left);
    static int receiveAllMessage(const socket_N& sock,char *message);

    static void closeSocket(socket_N& sock){
#ifdef Windows
        closeSocket(sock);
#else
        close(sock);
#endif
    }

    socket_N server_sock_;
    socket_N clnt_sock_;
private:
    sockaddr_in_N server_addr_;
    sockaddr_in_N client_addr_;
    socklen_t_N client_addr_sz;

    std::string ip_;
    unsigned int port_;
};


class socketNotes:public std::vector<std::pair<socket_N,std::string>> {
public:
    using std::vector<std::pair<socket_N,std::string>>::vector;
    /**
     * @brief 添加新套接字进入sockets中
     *
     * @param sock 套接字
     */
    inline void pushElement(socket_N sock){
        std::vector<std::pair<socket_N,std::string>>::push_back({sock,"no notes"});
    }
    /**
    * @brief 设置备注备注仅可为最后一次获取的套接字设置
    *
    * @param notes 备注信息
    */
    inline void setNotes(std::string notes){
        (this->end()-1)->second=std::move(notes);
    }
    /**
     * @brief 通过备注查询套接字
     *
     * @param notes 备注信息
     * @return 返回指向具有此备注的元素的迭代器,若无则返回尾部迭代器
     */
    [[nodiscard]] std::vector<std::pair<socket_N,std::string>>::iterator find(std::string notes){
        std::vector<std::pair<socket_N,std::string>>::iterator i=this->begin();
        for(;i!=this->end();i++){
            if(i->second==notes)return i;
        }
        return i;
    }
    /**
     * @brief 通过迭代器删除套接字
     *
     * @param i 指向将删除元素的迭代器
     */
    inline void eraseElement(std::vector<std::pair<socket_N,std::string>>::iterator i){
        std::vector<std::pair<socket_N,std::string>>::erase(i);
    }

};

/*此类仅用于数据的传输(public继承于SocketUtil)*/
class ServerSocketUtil:public SocketUtil{
public:
    ServerSocketUtil()=delete;
    explicit ServerSocketUtil(unsigned int port);
    ServerSocketUtil(const ServerSocketUtil&)=default;
    socket_N acceptSocket();
    ~ServerSocketUtil();

    socketNotes sockets_;
};

/*此类仅用于数据的传输(public继承于SocketUtil)*/
class ClientSocketUtil:public SocketUtil{
public:
    ClientSocketUtil()=delete;
    explicit ClientSocketUtil(const std::string& ip,unsigned int port);
    ~ClientSocketUtil();
};

//#ifdef IO_MULTIPLEXING
/*
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
        //FD_SET(server.server_sock_,&member_structure_.fd_set_);
        //++member_structure_.fd_nums;
        addFD(server.server_sock_);
    }
    void argumentSet(long timeout_seconds=5,long timeout_microseconds=5000);
private:
    void addFD(const socket_N& sock);
    void deleteFD(const socket_N& sock);
public:
    template<class T,class... Args>
    void simpleIOMultiplex(T,Args...);

    ServerSocketUtil server_socket_util_;
    MemberStructure member_structure_;
};
*/
#ifdef Windows_IOCP
class IOMultiplexIOCP{
public:

};
#endif

#ifdef Linux_EPOLL
class IOMultiplexEPOLL{
public:

};
#endif




#endif //LEI_NET_SOCKET_UTIL_H

//#ifdef IO_MULTIPLEXING

/**
 * @brief 进入IO复用模式,传递可调用数据与其参数,用以处理接收到的消息。新连接建立: something int,连接断开: close sock,设置的单次超时时间到达: timeout。
 *
 * @tparam T 可调用数据类型
 * @tparam Args 包名
 * @param function 可调用形参名
 * @param args 包形参名
 */
 /*
template<class T,class... Args>
void IOServerSocketUtil::simpleIOMultiplex(T function_,Args... args){
    int return_val;
    int len;
    while(true) {

        member_structure_.fd_set_cpy=member_structure_.fd_set_;
        member_structure_.timeout.tv_sec = member_structure_.timeout_seconds;
        member_structure_.timeout.tv_usec = member_structure_.timeout_microseconds;

        fd_set& fdset=member_structure_.fd_set_cpy;
        if((return_val= select(member_structure_.fd_nums,&fdset,
                               nullptr,nullptr,&member_structure_.timeout))==-1){
            lei_net_error::throwException("select error!",2);
            break;
        }
        if(return_val==0){std::puts("timeout");continue;}

        for(int i=0;i<fdset.fd_count;++i){
            if(FD_ISSET(fdset.fd_array[i],&fdset)){
                socket_N sock=fdset.fd_array[i];
                if(sock==server_socket_util_.server_sock_){
                    socket_N clnt = server_socket_util_.clnt_sock_=server_socket_util_.acceptSocket();
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
                        function_(message);
                        //std::cout<<message<<'\n';
                    }
                }
            }
        }
    }
}
*/
//#endif