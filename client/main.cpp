/*@wanlu 2021-5*/
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<opencv2/opencv.hpp>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<errno.h>
#include<iostream>
class DartSocket{
    public:
        DartSocket();
        void _accept();
    private:
        int _server_socket;//服务端套接字
        int _client_socket;//客户端套接字
        struct sockaddr_in _server_address;//服务端信息
        struct sockaddr_in _client_address;//客户端信息
        socklen_t _client_address_size;
        char _buffer[512];
};
DartSocket::DartSocket(){
    //创建本机套接字
   _client_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   if(_client_socket==-1){
       printf("socket faild ,errno = %d.\n",errno);
       exit(-1);
   }
   //主机地址信息
   memset(&_server_address,0,sizeof(_server_address));
   _server_address.sin_family=AF_INET;
   _server_address.sin_addr.s_addr=inet_addr("10.60.66.152");
   _server_address.sin_port=htons(1234);
   if(0!=connect(_client_socket,(struct sockaddr*)&_server_address,sizeof(_server_address))){
       printf("connect failed,errno = %d.\n",errno);
       exit(-1);
   }
   //std::cout<<"连接服务器成功\n";
}

void DartSocket::_accept(){
    char *imgdata=NULL;

    imgdata=(char*)malloc(sizeof(char)*(320*240));
    if(imgdata==NULL){
        return;
    }
    size_t now=0;
    while(1){
        int len=read(_client_socket,_buffer,512);
        if(now==320*240){//show
            //std::cout<<"接受一张图片\n";
            //定义图像
            cv::Mat greyMat(240,320,CV_8UC1,cv::Scalar(255,255,255));
            size_t temp_now=0;
            for(int i=0;i<greyMat.rows;i++){
                for(int j=0;j<greyMat.cols;j++){
                    greyMat.at<uchar>(i,j)=imgdata[temp_now++];
                }
            }
            cv::imshow("show",greyMat);
            cv::waitKey(1);
            close(_client_socket);
            free(imgdata);
            return;
        }
        if(len==-1){
            close(_client_socket);
            free(imgdata);
            return;
        }
        if(len>0){
            for(int i=0;i<len;i++){
                imgdata[now++]=_buffer[i];
            }
        }
    }
    free(imgdata);
    close(_client_socket);
}



int main(void){
    cv::namedWindow("show",0);
    while(1){
        DartSocket dartsocket;
        dartsocket._accept();
    }
    return 0;
}
