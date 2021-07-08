#ifndef __SERIAL_H__
#define __SERIAL_H__
#include<string.h>
#include<unistd.h>
#include<termios.h>
#include<sys/ioctl.h>
#include<iostream>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<termios.h>
#include<vector>
#include<opencv2/opencv.hpp>
class Serial{
public:
    Serial();//构造
    ~Serial();//析构
    int send(std::vector<cv::Point>result);//发送数据接口
    int init();
private:
    struct termios _opt;//串口配置信息
    int _serial;//串口文件id
    const char BEFORE = 0XAA;//帧头
    const char AFTER = 0xAF;//帧尾
    const char NUMBER = 0x08;
    char _buffer[70];//缓冲区
};
#endif
