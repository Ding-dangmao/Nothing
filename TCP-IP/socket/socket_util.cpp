#include "socket_util.hpp"

/*(基类)*/

/**
 * @description: 基类构造
 * @param {std::string} ip IP地址值
 * @param {unsigned} port 端口值
 */
SocketUtil::SocketUtil(std::string ip,unsigned int port):port_(port),ip_(std::move(ip)){}

/**
 * @description: sendMessage重载1,用于传输数据
 * @param {std::string} 被传输字符串
 */
int SocketUtil::sendMessage(const std::string& message)const{
    int len = static_cast<int>(message.size());
    //send(clnt_sock_, message.c_str(), len, 0);
    //send(clnt_sock_, message.c_str(), len,0);
    SEND_MESSAGE(clnt_sock_,message,len,0);
    return len;
}
/**
 * @description: sendMessage重载2,用于指定套接字传输数据
 * @param {int} sock 目标套接字
 * @param {std::string} 被传输字符串
 */
int SocketUtil::sendMessage(const socket_N& sock,const std::string& message) {
    int len = static_cast<int>(message.size());
    //send(sock, message.c_str(), len,0);
    SEND_MESSAGE(sock,message,len,0);
    return len;
}
/**
 * @description: receiveMessage重载1,用于接收数据
 * @param {char*} 接收数据字符串
 */
int SocketUtil::receiveMessage(char* message)const {
    //int len = recv(clnt_sock_, message, DATA_MAX_SIZE,0);
    int len= RECEIVE_MESSAGE(clnt_sock_,message,DATA_MAX_SIZE-1,0);
    if(len==-1)
        lei_net_error::throwException("socket/socket_util recv error!",2);
    return len;
}
/**
 * @description: receiveMessage重载2,用于接收数据,决定数据接收长度与边界
 * @param {char*} 接收数据字符串
 * @param {int} 接收长度
 * @param {unsigned int} 接收左边界
 * @param {unsigned int} 接收右边界
 */
int SocketUtil::receiveMessage(char* message,int lenn,unsigned int left)const {
    //int len = recv(clnt_sock_, message+left,lenn,0);
    int len= RECEIVE_MESSAGE(clnt_sock_,message+left,lenn,0);
    return len;
}

/**
 * @description: receiveMessage重载3,用于接收数据,指定接收数据的套接字
 * @param {char*} 接收数据字符串
 */
int SocketUtil::receiveMessage(const socket_N& sock,char* message) {
    //int len = recv(sock, message, sizeof(message)-1,0);
    int len= RECEIVE_MESSAGE(sock,message,DATA_MAX_SIZE-1,0);
    return len;
}
/**
 * @description: receiveMessage重载4,用于接收数据,指定接收数据的套接字,决定数据接收长度与边界
 * @param{int} sock
 * @param {char*} message 接收数据字符串
 * @param {int} 接收长度
 * @param {unsigned int} 接收左边界
 * @param {unsigned int} 接收右边界
 */
int SocketUtil::receiveMessage(const socket_N& sock, char* message,int lenn, unsigned int left) {
    //int len = recv(sock, message+left,lenn,0);
    int len= RECEIVE_MESSAGE(sock,message+left,lenn,0);
    return len;
}

int SocketUtil::receiveAllMessage(char* message)const{
    char length_char[TINY_CHAR];
    unsigned int left = 0; int len{};

    while (true) {
        int val=receiveMessage(length_char, 1, left);
        if(val==0)return 0;
        if(val==-1){
            lei_net_error::throwException("util/socket/socket_util.cpp socket error , check close sock",1);
        }
        left++;
        if (length_char[left - 1] == '/')break;
        if (left > 10) {
            lei_net_error::throwException("util/socket/socket_util.cpp-len socket read len error! please check Whether the transmission format meets the requirements ",1);
        }
    }

    int lleft{};
    while (lleft<left-1) {
        len = len * 10 + length_char[lleft++] - '0';
    }
    int single_len{}, receive_len{}, cnt{};
    while (receive_len != len) {
        single_len=receiveMessage(message, len - receive_len, receive_len);
        receive_len += single_len;
        cnt++;
        if (cnt > 1e5)
            lei_net_error::throwException("util/socket/socket_util.cpp-message socket read string error! This is a source code error!", 1);
    }

    message[len]=0;
    return len;
}


/**
 * @description: 此函数获得数据为无标识长度数据不可直接传输
 *
 * @param{int} sock
 * @param {char*} message 接收数据字符串
 */
int SocketUtil::receiveAllMessage(const socket_N& sock,char *message){
    char length_char[TINY_CHAR];
    unsigned int left{}; int len{};

    while (true) {
        int val=receiveMessage(sock,length_char, 1, left);
        //std::cout<<val<<std::endl;
        if(val==0)return 0;
        if(val==-1){
            lei_net_error::throwException("util/socket/socket_util.cpp socket error : check close sock",2);
        }
        left++;
        if (length_char[left - 1] == '/')break;
        if (left > 10) {
            lei_net_error::throwException("util/socket/socket_util.cpp-len socket read len error! please check Whether the transmission format meets the requirements ",2);
        }
    }

    int lleft{};
    while (lleft<left-1) {
        len = len * 10 + length_char[lleft++] - '0';
    }

    int single_len{}, receive_len{}, cnt{};
    while (receive_len != len) {
        single_len=receiveMessage(sock,message, len - receive_len, receive_len);
        receive_len += single_len;
        cnt++;
        if (cnt > 1e5)
            lei_net_error::throwException("util/socket/socket_util.cpp-message socket read string error! This is a source code error!", 2);
    }

    message[len]=0;
    return len;
}

/*ServerSocketUtil*/

/**
 * @description: Server构造
 * @param {unsigned} port 开放端口值
 */
ServerSocketUtil::ServerSocketUtil(const unsigned int port): SocketUtil("NULL",port) {
    if((server_sock_=socket(PF_INET,SOCK_STREAM,0))==INVALID_SOCKET_N)
        lei_net_error::throwException("util/socket_util.cpp server socket() create error!",2);

    memset(&server_addr_,0,sizeof(server_addr_));
    server_addr_.sin_family=AF_INET;
    server_addr_.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr_.sin_port=htons(port);

    if(bind(server_sock_,(sockaddr_N*)&server_addr_,sizeof(server_addr_))==SOCKET_ERROR_N)
        lei_net_error::throwException("util/socket_util.cpp server bind() create error!",2);
    if(listen(server_sock_,5)==SOCKET_ERROR_N)
        lei_net_error::throwException("util/socket_util.cpp server listen() create error!",2);

}

/**
 * @description: 监听函数,监听客户端(相对关系)的连接请求
 */
socket_N ServerSocketUtil::acceptSocket(){
    //待定
    //SOCKET client_sock; b b 
    client_addr_sz=sizeof(client_addr_);
    if((clnt_sock_=accept(server_sock_,(sockaddr_N*)&client_addr_,&client_addr_sz))==INVALID_SOCKET_N)
        lei_net_error::throwException("util/socket_util.cpp server accept() create error!",2);

    sockets_.pushElement(clnt_sock_);
    return clnt_sock_;
}
/**
 * @brief 设置套接字备注
 *
 * @param notes 备注
 */


ServerSocketUtil::~ServerSocketUtil(){
    //信息传递套接字需考虑
#ifdef Windows
    closesocket(server_sock_);
#else
    close(server_sock_);
#endif
}



//#ifdef IO_MULTIPLEXING
/*IOServerSocketUtil*/
/*
void IOServerSocketUtil::argumentSet(long timeout_seconds,long timeout_microseconds){
    member_structure_.timeout_seconds=timeout_seconds;
    member_structure_.timeout_microseconds=timeout_microseconds;
}
 */
/**
 *  @brief  将新建立连接的套接字注册
 *
 * @param sock  新连接的套接字
 */
 /*
void IOServerSocketUtil::addFD(const SOCKET& sock){
    FD_SET(sock,&member_structure_.fd_set_);
    ++member_structure_.fd_nums;
}
  */

/**
 * @brief 将断开的套接字删除
 *
 * @param sock 断开的套接字
 */
 /*
void IOServerSocketUtil::deleteFD(const socket_N& sock){
    FD_CLR(sock,&member_structure_.fd_set_);
    --member_structure_.fd_nums;
}
  */
//#endif

/*ClientSocketUtil*/

/**
 * @description: Client构造
 * @param {unsigned} ip 目标服务器IP地址值 默认值为Linux端服务器IP
 * @param {unsigned} port 目标服务器端口值 默认值为Linux端开放的9190端口
 */
ClientSocketUtil::ClientSocketUtil(const std::string& ip,const unsigned int port): SocketUtil(ip,port){
    if((clnt_sock_=socket(PF_INET,SOCK_STREAM,0))==INVALID_SOCKET_N)
        lei_net_error::throwException("util/socket_util.cpp client socket() create error!",2);

    memset(&server_addr_,0,sizeof(server_addr_));
    server_addr_.sin_family=AF_INET;
    server_addr_.sin_addr.s_addr= inet_addr(ip.c_str());
    server_addr_.sin_port=htons(port);

    if(connect(clnt_sock_,(sockaddr_N*)&server_addr_,sizeof(server_addr_))==SOCKET_ERROR_N)
        lei_net_error::throwException("util/socket_util.cpp client connect() error!",2);
}
ClientSocketUtil::~ClientSocketUtil(){
#ifdef Windows
    closesocket(server_sock_);
#else
    close(server_sock_);
#endif
}