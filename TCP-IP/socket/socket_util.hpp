#ifndef LEI_NET_SOCKET_UTIL_H
#define LEI_NET_SOCKET_UTIL_H

#include<iostream>
#include<algorithm>
#include<vector>
#include<cmath>
#include<cstring>

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
        closesocket(sock);
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

class IOServerSocketUtil{
public:
    struct MemberStructure {
        MemberStructure():fd_nums(0),fd_num_max(0){}
        fd_set fd_set_;
        fd_set fd_set_cpy;
        timeval timeout;
        long timeout_seconds,timeout_microseconds;
        int fd_nums;
        int fd_num_max;
    };
public:
    IOServerSocketUtil()=delete;
    inline explicit IOServerSocketUtil(ServerSocketUtil& server): member_structure_(),server_socket_util_(server){
        FD_ZERO(&member_structure_.fd_set_);
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

#ifdef Windows
#define WSA_MAXIMUM_SOCKETS 110
#define WSA_MAXIMUM_EVENTS 110
class AsyncNotifyIO{
public:
    AsyncNotifyIO()=delete;
    explicit inline AsyncNotifyIO(ServerSocketUtil& server): member_structure_(), server_socket_util_(server){
        member_structure_.new_event_=WSACreateEvent();
        if(WSAEventSelect(server_socket_util_.server_sock_,member_structure_.new_event_,FD_ACCEPT)==SOCKET_ERROR_N)
            lei_net_error::throwException("socket_util.hpp AsyncNotifyIO initialize error",2);
        member_structure_.socket_set_[member_structure_.socket_nums_]=server_socket_util_.server_sock_;
        member_structure_.event_set_[member_structure_.socket_nums_]=member_structure_.new_event_;
        ++member_structure_.socket_nums_;
    }
    struct MemberStructure {
        inline MemberStructure():socket_nums_(0){}
        socket_N socket_set_[WSA_MAXIMUM_SOCKETS]{};
        WSAEVENT event_set_[WSA_MAXIMUM_EVENTS];
        WSAEVENT new_event_;
        WSANETWORKEVENTS net_work_events_;
        int socket_nums_;
    };

    template<typename T,typename ... Arg>
    bool operator()(T,Arg...);

    ServerSocketUtil server_socket_util_;
    MemberStructure member_structure_;
};
#endif


#ifdef Linux
class IOMultiplexEPOLL{
public:

};
#endif




#endif //LEI_NET_SOCKET_UTIL_H

/**
 * @brief 进入IO复用模式,传递可调用数据与其参数,用以处理接收到的消息。新连接建立: something int,连接断开: close sock,设置的单次超时时间到达: timeout。
 *
 * @tparam T 可调用数据类型
 * @tparam Args 包名
 * @param function 可调用形参名
 * @param args 包形参名
 */

template<class T,class... Args>
void IOServerSocketUtil::simpleIOMultiplex(T function_,Args... args){
    int return_val;
    int len;
    while(true) {

        member_structure_.fd_set_cpy=member_structure_.fd_set_;
        member_structure_.timeout.tv_sec = member_structure_.timeout_seconds;
        member_structure_.timeout.tv_usec = member_structure_.timeout_microseconds;

        fd_set& fdset=member_structure_.fd_set_cpy;
#ifdef Windows
        if((return_val= select(member_structure_.fd_nums,&fdset,
                               nullptr,nullptr,&member_structure_.timeout))==-1){
            lei_net_error::throwException("select error!",2);
            break;
        }
#else
        if((return_val= select(member_structure_.fd_num_max+1,&fdset,
                               nullptr,nullptr,&member_structure_.timeout))==-1){
            lei_net_error::throwException("select error!",2);
            break;
        }
#endif
        if(return_val==0){std::puts("timeout");continue;}
#ifdef Windows
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
                        //closesocket(sock);
                        SocketUtil::closeSocket(sock);
                    }else{
                        //函数调用
                        function_(message);
                        //std::cout<<message<<'\n';
                    }
                }
            }
        }
#else
        for(int i=0;i<=member_structure_.fd_num_max;++i){
            if(FD_ISSET(i,&fdset)){
                socket_N sock= i;
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
                        SocketUtil::closeSocket(sock);
                    }else{
                        //函数调用
                        function_(message);
                        //std::cout<<message<<'\n';
                    }
                }
            }
        }
#endif
    }
}

#ifdef Windows
template<typename T,typename... Arg>
bool AsyncNotifyIO::operator()(T F,Arg...) {
    unsigned long long return_id;
    unsigned long long start_id;

    socket_N* socket_set=member_structure_.socket_set_;
    WSAEVENT* event_set=member_structure_.event_set_;
    WSAEVENT& new_event=member_structure_.new_event_;
    WSANETWORKEVENTS& net_work_event=member_structure_.net_work_events_;
    int& socket_nums=member_structure_.socket_nums_;
    while(1){
        return_id= WSAWaitForMultipleEvents(socket_nums,event_set,
                                            FALSE,WSA_INFINITE,
                                            FALSE);
        start_id=return_id-WSA_WAIT_EVENT_0;

        for(unsigned long long i=start_id;i<socket_nums;i++){
            unsigned long long single_id= WSAWaitForMultipleEvents(1,&event_set[i],
                                                                   FALSE,0,
                                                                   FALSE);
            if(single_id==WSA_WAIT_FAILED || single_id==WSA_WAIT_TIMEOUT)
                continue;
            else{
                single_id=i;
                WSAEnumNetworkEvents(socket_set[single_id],event_set[single_id],
                                     &net_work_event);
                if(net_work_event.lNetworkEvents & FD_ACCEPT){
                    if(net_work_event.iErrorCode[FD_ACCEPT_BIT]!=0)
                    {
                        lei_net_error::throwException("socket_util AsyncNotifyIO Accept error");
                    }
                    server_socket_util_.clnt_sock_=server_socket_util_.acceptSocket();
                    new_event= WSACreateEvent();
                    WSAEventSelect(server_socket_util_.clnt_sock_,new_event,
                                   FD_READ);

                    event_set[socket_nums]=new_event;
                    socket_set[socket_nums]=server_socket_util_.clnt_sock_;
                    socket_nums++;
                    SocketUtil::sendMessage(server_socket_util_.clnt_sock_,"20/receive your message");
                    std::puts("connected new client");
                }

                if(net_work_event.lNetworkEvents & FD_READ){
                    if(net_work_event.iErrorCode[FD_READ_BIT]!=0){
                        lei_net_error::throwException("socket_util.hpp AsyncNotifyIO READ error",2);
                    }
                    char message[DATA_MAX_SIZE];
                    //函数处理
                    SocketUtil::receiveAllMessage(socket_set[single_id],message);
                    F(message);
                }

                if(net_work_event.lNetworkEvents & FD_CLOSE){
                    std::puts("close socket");
                    if(net_work_event.iErrorCode[FD_CLOSE_BIT]!=0){
                        lei_net_error::throwException("socket_util.hpp AsyncNotifyIO CLOSE error",2);
                    }
                    WSACloseEvent(event_set[single_id]);
                    SocketUtil::closeSocket(socket_set[single_id]);

                    //除去
                    socket_nums--;
                    std::swap(event_set[single_id],event_set[socket_nums]);
                    std::swap(socket_set[single_id],socket_set[socket_nums]);

                }
            }
        }
    }
}

#endif
