/* Linux Serial Port Model */
//动态库依赖:pthread
/*
 在Linux中,设备文件一般位于/dev下，
 其中串口1、串口2对应文件为/dev/ttyS0 、/dev/ttyS1
 串口通信过程
 打开串口设备文件、获取设备句柄fd
 设置串口参数，如波特率、数据位、奇偶校验位、停止位等等
 利用select函数等待串口设备是否可读写（有数据）
 应用read、write函数读写串口
 注意：串口读写权限
*/
#include"SerialTool.h"

//串口线程函数 接受参数为SerialTool对象指针
void SERIALTOOL_THREAD_FUNCTION(void*serialObject){
    printf("SerialTool::开启串口读线程\n");
    SerialTool*obj=(SerialTool*)serialObject;
    int size=obj->getSize();
    char* buffer=(char*)obj->getBuffer();
    //不断地读取数据有数据就读到buffer内
}

//打开串口设备
int SerialTool::openPort(const char*serialPath){
    int fd=-1;
    fd=open(serialPath,O_RDWR|O_NOCTTY|O_NDELAY);
    /*O_RDWR 可以读写
     *O_NOCTTY 该参数不会使打开的文件成为该进程的控制终端。
     *         如果没有指定这个标志，那么任何一个 输入都将会影响用户的进程。
     *O_NDELAY 这个程序不关心DCD信号线所处的状态,
     *         端口的另一端是否激活或者停止。如果用户不指定了这个标志，
     *         则进程将会一直处在睡眠状态，直到DCD信号线被激活。
     * */
    if(-1==fd){
        perror("[SerialTool::openPort] 打开串口失败");
        return -1;
    }
    return fd;//打开串口成功返回设备号
}

//串口参数设置
//nSpeed:波特率 nBits:数据位数 nEvent:奇偶校验 nStop:停止位
int SerialTool::setSerialParam(int nSpeed,int nBits,char nEvent,int nStop){
    struct termios newtio,oldtio;
    //获取现有串口参数
    if(tcgetattr(fd,&oldtio)!=0){
        perror("[SerialTool::openPort] 获取串口现有参数失败");
        return -1;//失败
    }
    bzero(&newtio,sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag&=~CSIZE;

    switch(nBits){//数据位
        case 7:newtio.c_cflag|=CS7;break;
        case 8:newtio.c_cflag|=CS8;break;
        default:break;
    }
    switch(nEvent){//奇偶无校验
        case 'O':newtio.c_cflag|=PARENB|PARODD;
                 newtio.c_iflag|=(INPCK|ISTRIP);
                 break;
        case 'E':newtio.c_iflag|=(INPCK|ISTRIP);
                 newtio.c_cflag|=PARENB;
                 newtio.c_cflag&=~PARODD;
                 break;
        case 'N':newtio.c_cflag&=~PARENB;
                 break;
        default:break;
    }
    //设置波特率
    switch(nSpeed){
        case 9600:cfsetispeed(&newtio,B9600);
                  cfsetospeed(&newtio,B9600);
                  break;
        case 115200:cfsetispeed(&newtio,B115200);
                    cfsetospeed(&newtio,B115200);
                    break;
        default:break;
    }
    //停止位
    if(nStop==1){
        newtio.c_cflag &= ~CSTOPB;
    }else if(nStop==2){
        newtio.c_cflag |= CSTOPB;
    }
    newtio.c_cc[VTIME]=0;
    newtio.c_cc[VMIN]=0;
    tcflush(fd,TCIFLUSH);
    //设置参数
    if((tcsetattr(fd,TCSANOW,&newtio))!=0){
        perror("[SerialTool::openPort] 设置串口参数失败");//设置失败
        return -1;
    }
    //设置完毕
    printf("[SerialTool::openPort] 设置串口参数成功]");
    return 0;
}

//关闭串口
void  SerialTool::closeSerial(){
    close(fd);
}

//初始化
int SerialTool::init(void*buffer,int size,const char*path,int nSpeed,int nBits,int nEvent,int nStop){
    fd_set rd;   
    this->fd=fd;
    this->nSpeed=nSpeed;
    this->nBits=nBits;
    this->nEvent=nEvent;
    this->nStop=nStop;
    //打开串口
    if(openPort(path)==-1){
        return 0;
    }
    //设置串口参数
    if(setSerialParam(this->fd,this->nSpeed,this->nBits,this->nEvent,this->nStop)!=0){
        return 0;//设置串口参数失败
    }
    //开启串口线程
    pthread_create(&this->threadId,NULL,SERIALTOOL_THREAD_FUNCTION,this);
    return 1;//初始化成功

    // FD_ZERO(&rd);
    // FD_SET(fd,&rd);
    // while(FD_ISSET(fd,&rd)){
    //     //检测未见标识符的IO有没有准备好,检测有没有数据
    //     if(select(fd+1,&rd,NULL,NULL,NULL)<0){
    //         perror("select");
    //     }else{//没有数据不会执行else代码块
    //         memset(buff,0,128);
    //         while((nread=read(fd,buff,128))>0){
    //             printf("数据共%d字节|内容:%s\n",nread,buff);
    //             write(fd,buff,strlen(buff));//会送数据
    //         }
    //     }
    // }

    return 0;
}

//构造函数
SerialTool::SerialTool(){
    fd=-1;

}

//析构函数
SerialTool::~SerialTool(){
    if(fd!=-1){
        close(fd);
    }
}


//发送数据接口
int SerialTool::writeData(void*data,int size){
    return 0;
}


void* SerialTool::getBuffer(){
    return this->buffer;
}

int SerialTool::getSize(){
    return this->size;
}



// //串口线程函数
// void* serial_thread(void *d){
//     //获得串口文件标识符
//     int i=0;
//     int tfd=*(int*)d;
//     if(tfd>0){
//         while(1){
//             char buf[20],str[]="oooooooooo";
//             if(i<10){
//                 str[i]='*';
//             }else{
//                 str[19-i]='*';
//             }
//             i%=20;
//             sprintf(buf,"%s\r\n",str);
//             write(tfd,buf,strlen(buf));
//             usleep(100000);//延时100ms
//         }
//     }
//     return 0;
// }


// //主函数
// int main(int argc,char**argv){
//     int fd,nread,i;
//     char buff[128]="Hello\n";
//     fd_set rd;
//     if((fd=open_port(2))<0){//打开串口
//         perror("open_port error");
//         return -1;
//     }
//     //设置串口参数
//     if((i=set_opt(fd,9600,8,'N',1))<0){
//         perror("set_opt error");
//         return -1;
//     }
//     //打开串口成功后，开启串口线程
//     printf("开启串口线程\n");
//     pthread_t id;
//     pthread_create(&id,NULL,serial_thread,&fd);

//     FD_ZERO(&rd);
//     FD_SET(fd,&rd);
//     while(FD_ISSET(fd,&rd)){
//         //检测未见标识符的IO有没有准备好,检测有没有数据
//         if(select(fd+1,&rd,NULL,NULL,NULL)<0){
//             perror("select");
//         }else{//没有数据不会执行else代码块
//             memset(buff,0,128);
//             while((nread=read(fd,buff,128))>0){
//                 printf("数据共%d字节|内容:%s\n",nread,buff);
//                 write(fd,buff,strlen(buff));//会送数据
//             }
//         }
//     }
//     close(fd);
//     return 0;
// }