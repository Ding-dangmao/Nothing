# TCP-IP

此文件简单封装了TCP-IP相关的函数功能

## 类结构

**套接字句柄**

基类SocketUtil存储最近一次使用的套接字句柄(包括Client,还有Server)。

若是ServerSocketUtil则必存储`server_sock_`还有最近一次使用的`client_sock_`

若是ClientSocketUtil则只存储`client_sock_`

**地址值**

基类SocketUtil存储最近一次使用的套接字句柄(包括Client,还有Server)。

基类SocketUtil存储最近一次使用的套接字句柄(包括Client,还有Server)。

若是ServerSocketUtil则必存储`server_addr_`还有最近一次使用的`client_addr_`

若是ClientSocketUtil则只存储`client_addr_`

**IP/PORT**

IP值在ServerSocketUtil有所冗余

ServerSocketUtil保存开放端口

ClientSocketUtil保存自身IP值还有目标Server的开放端口

## 函数相关

~~~C++
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
~~~

有两大类,默认套接字句柄/指定套接字句柄

receiveAllMessage是由receiveMessage的带有指定读取长度重载版本编写而成

## 传输格式

传输格式为:   `长度/字符串`

长度功能为指定字符串的长度,此要求在使用receiveAllMessage时必须遵守,否则会抛出异常

> cmake配置
>
> ```
> link_libraries(ws2_32.lib)
> ```
>
> 此库是必要的

## 必要

~~~C++
int main(){
    WSADATA wsadata;
    {
        //在此内进行,若与wsadata作用域相同,套接字close时会发生错误
    }
    WSACleanup();
}
~~~

## 拓展类

### IOServerSocketUtil

~~~C++
    inline explicit IOServerSocketUtil(ServerSocketUtil& server): member_structure_(),server_socket_util_(server){
        FD_ZERO(&member_structure_.fd_set_);
        FD_SET(server.server_sock_,&member_structure_.fd_set_);
        ++member_structure_.fd_nums;
    }
~~~

​	简单封装了I/O复用,可以直接调用来达到效果,其构造函数接收一个ServerSocketUtil参数,并拷贝。

#### 函数

~~~C++
template<class T>
void simpleIOMultiplex(T);
~~~

接收一个必须接受字符串的函数(std::string),函数返回值没有要求,只有参数要求

## 待写传输

![](.\img\传输.png)

## 难以处理的点

​	由于套接字的使用方式,互相的关联性不可预测,对于部分套接字的销毁难以处理所以在对于**ServerSocketUtil**类的析构函数处理中并未对其接受到的套接字进行任何处理,而是交由使用者使用**static void closeSocket(SOCKET& sock)**这一静态成员函数进行自发处理

