#ifndef LEI_NET_ERRORS_H
#define LEI_NET_ERRORS_H

#include<iostream>

#include<winsock2.h>
/* 错误模版,用于为输出错误提供帮助 */
class Error {
public:
    Error(): msg_("Error: "){}
    explicit Error(const std::string& msg):msg_("Error:"+msg){}
    const char* what()const noexcept{return msg_.c_str(); }
private:
    std::string msg_;
};

/* public继承于Error,功能相似 */
class SocketError:public Error{
public:
    explicit SocketError(const std::string msg):Error(msg+"\t*(Socket)*\n"){}
};

/*抛出错误*/
namespace lei_net_error {
/**
 * @description: 接受错误信息,抛出并打印错误
 * @param {std::string} msg 错误信息
 * @param {int} type 错误所属类型,2:SocketError
 */
    inline void throwException(const std::string error_msg,int type)try{
        if(type==2) throw SocketError(error_msg);
        else throw nullptr;
    }catch(const SocketError& e){
        std::cout<<e.what();
        system("pause");
        exit(1);
    }catch(...){
        std::cout<<"\tUnknown\n";
        system("pause");
        exit(1);
    }
/**
 * @description: 接受错误信息,抛出并打印错误
 * @param {std::string} msg 错误信息
 */
    inline void throwException(const std::string error_msg)try{
        throw Error(error_msg);
    }catch(const Error& e){
        std::cout<<e.what();
        system("pause");
        exit(1);
    }catch(...){
        std::cout<<"\tUnknown\n";
        system("pause");
        exit(1);
    }
} //namespace lei_net_error

#endif //LEI_NET_ERRORS_H
