/*@wanlu 2021-5*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<error.h>

#define SERVER_IP "10.0.1.5"
#define SERVER_PORT 80
#define USERID "1901420313"
#define PASSWORD "221347"



//发起GET请求
void GetRequest(void*socket);
//接受响应
void GetResponse(void*socket);
//主函数
int main(void){
    printf("桂林电子科技大学校园网络登录(Linux)\n");
    printf("程序由高万禄提供(2209120827@qq.com)\n");
    printf("软件声明:此软件开发者不承担任何责任\n");
    printf("开源协议:MIT\n");
    //请求头格式
    //GET /drcom/login?callback=dr1003&DDDDD=1901420313&upass=789567&0MKKey=123456&R1=0&R2=&R3=0&R6=0&para=00&v6ip=&terminal_type=1&lang=zh-cn&jsVerion=4.1&v=5455&lang=zh HTTP/1.1
    //Host: 10.0.1.5
    //Connection: keep-alive
    //Cache-Control: max-age=0
    //Upgrade-Insecure-Requests: 1
    //User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 13_2_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.3 Mobile/15E148 Safari/604.1
    //Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
    //Accept-Encoding: gzip, deflate
    //Accept-Language: zh-CN,zh-TW;q=0.9,zh;q=0.8,en-US;q=0.7,en;q=0.6,und;q=0.5
    
    //客户端需要创建两个线程,一个线程向套接子写入内容,一个线程读取套接字内容，二者同时进行

    //创建套接字
    int client_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(client_socket==-1){
        printf("创建套接字失败...\n");
        return -1;
    }else{
        printf("创建套接字成功...\n");
    }
    //服务端信息
    struct sockaddr_in server_address;
    memset(&server_address,0,sizeof(server_address));
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=inet_addr(SERVER_IP);
    server_address.sin_port=htons(SERVER_PORT);
    printf("正在连接服务器...\n");
    if(0!=connect(client_socket,(struct sockaddr*)&server_address,sizeof(server_address))){
        printf("连接服务器失败\n");
        return -1;
    }
    //请求线程
    pthread_t request;
    int request_id;
    request_id=pthread_create(&request,NULL,(void*)&GetRequest,(void*)&client_socket);
    //响应线程
    pthread_t response;
    int response_id;
    response_id=pthread_create(&response,NULL,(void*)&GetResponse,(void*)&client_socket);
    //阻塞
    void *retavl;
    pthread_join(request,&retavl);
    pthread_join(response,&retavl);
    close(client_socket);
    printf("程序结束\n");
    return 0;
}

//发起GET请求
void GetRequest(void *socket){
    int client_socket=*((int*)socket);
    printf("套接字%d\n",client_socket);
    //创建临时写入文件
    printf("正在创建临时HTPP请求头文件\n");
    FILE*HTTP_FILE=NULL;
    if((HTTP_FILE=fopen("./HTTP_FILE","w"))==NULL){
        printf("临时文件创建失败\n");
        return;
    }
    //发起GET请求
    fputs("GET ",HTTP_FILE);
    //请求信息
    fputs("/drcom/login?callback=dr1003&DDDDD=",HTTP_FILE);
    fputs(USERID,HTTP_FILE);
    fputs("&upass=",HTTP_FILE);
    fputs(PASSWORD,HTTP_FILE);
    fputs("&0MKKey=123456&R1=0&R2=&R3=0&R6=0&para=00&v6ip=&terminal_type=1&lang=zh-cn&jsVerion=4.1&v=5455&lang=zh ",HTTP_FILE);
    //HTTP1.1
    fputs("HTTP/1.1\n",HTTP_FILE);
    //Host
    fputs("Host: ",HTTP_FILE);
    fputs(SERVER_IP,HTTP_FILE);
    fputs("\n",HTTP_FILE);
    //Connection
    fputs("Connection: keep-alive\n",HTTP_FILE);
    //Cache-Control
    fputs("Cache-Control: max-age=0\n",HTTP_FILE);
    //Upgrade-Insecure-Requests
    fputs("Upgrade-Insecure-Requests: 1\n",HTTP_FILE);
    //User-Agent
    fputs("User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 13_2_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.3 Mobile/15E148 Safari/604.1\n",HTTP_FILE);
    //Accept
    fputs("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n",HTTP_FILE);
    //Accept-Encoding
    fputs("Accept-Encoding: gzip, deflate",HTTP_FILE);
    //Accept-Language
    fputs("Accept-Language: zh-CN,zh-TW;q=0.9,zh;q=0.8,en-US;q=0.7,en;q=0.6,und;q=0.5\r\n\r\n",HTTP_FILE);
    printf("临时文件创建完毕\n");
    fclose(HTTP_FILE);//关闭文件
    //以读的形式重新打开文件
    HTTP_FILE=NULL;
    if((HTTP_FILE=fopen("./HTTP_FILE","r"))==NULL){
        printf("临时文件打开错误\n");
        return;
    }
    //读取文件
    char buffer[1]={0};
    char ch;
    while(ch=fgetc(HTTP_FILE)){
        if(feof(HTTP_FILE))break;
        buffer[0]=ch;
        printf("%c",buffer[0]);
        //写入到套接字
        write(client_socket,buffer,1);
    }
    printf("数据发送完毕\n");
    //关闭向服务端写数据通道
    shutdown(client_socket,SHUT_WR);
    fclose(HTTP_FILE);//关闭文件
    printf("请求发送完毕\n");
}


//接收响应
void GetResponse(void*socket){
    int client_socket=*((int*)socket);
    printf("套接字%d\n",client_socket);
    //从套接字内读取数据
    printf("创建临时输出文件\n");
    FILE *GET_FILE=NULL;
    if((GET_FILE=fopen("./GET_FILE","w"))==NULL){
        printf("临时输出文件创建失败\n");
        return;
    }
    //定义缓冲区
    char buffer[512]={0};
    size_t len;
    printf("接受数据中\n");
    while(0!=(len=read(client_socket,buffer,512))){
       fwrite(buffer,len,1,GET_FILE);
    }
    printf("接受数据完毕\n");
    fclose(GET_FILE);
    //以读的形式打开文件
    GET_FILE=NULL;
    if((GET_FILE=fopen("./GET_FILE","r"))==NULL){
        printf("打开临时输出文件失败\n");
        return;
    }
    char ch;
    while(ch=fgetc(GET_FILE)){
        if(feof(GET_FILE))break;
        printf("%c",ch);
    }
    printf("\n");
    fclose(GET_FILE);
}
