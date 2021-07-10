/*By WanluGao @2021 2209120827@qq.com*/
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Camara/Camara.h"
#include "GreenSpot/GreenSpot.h"
#include "Serial/SerialTool.h"
#include "Serial/SerialPack.h"
#include "DartSocket/DartSocket.h"
#include <stdlib.h>
#include <vector>

//Global Variable
//Serial ComBuffer Thread Lock
pthread_mutex_t SERIALTOOL_THREAD_COMBUFFER_MUTEX=PTHREAD_MUTEX_INITIALIZER;


int main(void)
{
    //IMPORT LOCK
    unsigned char databuffcom[11];//comhave space 
    cv::Mat ORIGINMAT; //帧图
    //打开摄像头
    std::cout << "[main.cpp::main] 正在打开相机\n";
    cv::VideoCapture capture(0);
    //摄像头是否打开成功
    if (!capture.isOpened())
    {
        std::cout << "[main.cpp::main] 相机打开失败\n";
        //return -1;
    }
    //相机参数设置
    //CamaraParam camaraParam;
    //std::cout << "[main.cpp::main] 正在设置相机参数\n";
    //camaraParam.seting(capture);

    //开启串口
    GreenSpot greenSpot;               //目标识别
    DartSocket dartsocket;             //套接字图传
    std::vector<cv::Point> targetData; //用于目标识别返回数据

    //调试窗口
    //cv::namedWindow("img", 0);
    //cv::namedWindow("bin", 0);

    std::cout << "[main.cpp::main] 相机视频流启动\n";

    unsigned char senddata[11];
    int tempcount=0;
    SerialTool tool;
    tool.init(databuffcom,11,"/dev/ttyS1",115200,8,'N',1);


    unsigned int last[4]={0,0,0,0};
    //Index Thread
    while(1){
        SerialToolPack pack;
        pack.x1=last[0];pack.y1=last[1];pack.x2=last[2];pack.y2=last[3];
        pack.pack(senddata,11);
    	tool.writeData(senddata,11);
        //LOCK
        pthread_mutex_lock(&SERIALTOOL_THREAD_COMBUFFER_MUTEX);
            pack.unpack(databuffcom,11);
        //UNLOCK
        pthread_mutex_unlock(&SERIALTOOL_THREAD_COMBUFFER_MUTEX);
        
        last[0]=pack.x1;last[1]=pack.y1;last[2]=pack.x2;last[3]=pack.y2;
        printf("UNPACK:x1=%u,y1=%u,x2=%u,y2=%u\n",pack.x1,pack.y1,pack.x2,pack.y2);

        usleep(100);
    }


    return 0;
}
