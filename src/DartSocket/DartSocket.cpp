/*@wanlu 2021-5*/
#include"DartSocket.h"
DartSocket::DartSocket(){
    //创建本机套接字
    _server_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(0>=_server_socket){
        printf("create socket failed,errno = %d.\n",errno);
    }
    //结构体初始化为0
    memset(&_server_address,0,sizeof(_server_address));
    _server_address.sin_family=AF_INET;
    _server_address.sin_addr.s_addr=inet_addr("10.60.67.251");
    _server_address.sin_port=htons(1234);
    //套接字绑定本机信息
    bind(_server_socket,(struct sockaddr*)&_server_address,sizeof(_server_address));
    //监听
    listen(_server_socket,20);
    _client_address_size=sizeof(_client_address);
    std::cout<<"监听成功\n";
}

void DartSocket::_accept(){
    //进行接受请求,获取客户端套接字client_socket
    //std::cout<<"accept\n";
    _client_socket=accept(_server_socket,(struct sockaddr*)&_client_address,&_client_address_size);
    //std::cout<<"客户端连接成功\n";
}

void DartSocket::_send(cv::Mat&img){
    if(img.empty()){
        return;
    }
    char*imgdata=NULL;
    imgdata=(char*)malloc(sizeof(char)*(img.rows*img.cols+4));
    if(imgdata==NULL){
        return;
    }
    cv::Mat greyMat;
    cv::cvtColor(img,greyMat,cv::COLOR_BGR2GRAY);
    size_t now=0;
    //遍历像素点
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            char temp=greyMat.at<uchar>(i,j);
            imgdata[now++]=temp;
        }
    }
    if(now>0){
        send(_client_socket,imgdata,now,0);
    }
    free(imgdata);
    //std::cout<<"发送成功\n";
    close(_client_socket);
}
