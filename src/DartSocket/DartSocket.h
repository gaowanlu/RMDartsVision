/*@wanlu 2021-5*/
#ifndef __DARTSOCKET_H__
#define __DARTSOCKET_H__
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<errno.h>
#include<opencv2/opencv.hpp>
#include<iostream>
class DartSocket{
    public:
        DartSocket();
        void _accept();
        void _send(cv::Mat&img);
    private:
        int _server_socket;//服务端套接字
        int _client_socket;//客户端套接字
        struct sockaddr_in _server_address;//本机信息
        struct sockaddr_in _client_address;//客户端信息
        socklen_t _client_address_size;
        char _buffer[512];
};
#endif
