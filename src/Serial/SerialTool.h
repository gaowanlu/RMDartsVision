#ifndef __SERIALTOOL_H__
#define __SERIALTOOL_H__
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<stdlib.h>
#include<pthread.h>

//串口线程函数
void SERIALTOOL_THREAD_FUNCTION(void*serialObject);

class SerialTool{
public:
    //@Constructor
    SerialTool();
    //关闭串口
    void closeSerial();
    //         读公共缓冲区  缓冲区大小 串口     波特率    数据位     奇偶校验    停止位 
    int init(void*buffer,int size,const char*path,int nSpeed,int nBits,int nEvent,int nStop);
    //发送数据接口
    int writeData(void*data,int size);
private:
    int fd;//串口设备标识符
    int nSpeed;//波特率
    int nBits;//数据位
    char nEvent;//奇偶校验
    int nStop;//停止位
    void *buffer;//公共缓冲区
    int size;//每帧数据字节数
    pthread_t threadId;
    /*
    * 打开串口
    * @param serialPath 串口文件绝对地址 
    */
    int openPort(const char*serialPath);
    /*
    * 设置串口参数
    * @param nSpeed 波特率 可选参数[9600\115200]
    * @param nBits 数据位 可选参数[7\8]
    * @param nEvent 奇偶校验 可选参数['O'\'E'\'N']
    * @param nStop 停止位 可选参数[1\2]
    */
    int setSerialParam(int nSpeed,int nBits,char nEvent,int nStop);
    void* getBuffer();
    int getSize();
};
#endif