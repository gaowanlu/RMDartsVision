/*By WanluGao @2021 2209120827@qq.com*/
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Camara/Camara.h"
#include "GreenSpot/GreenSpot.h"
#include "Serial/Serial.h"
#include "DartSocket/DartSocket.h"
#include <stdlib.h>
#include <vector>

int main(void)
{
    cv::Mat ORIGINMAT; //帧图
    //打开摄像头
    std::cout << "[main.cpp::main] 正在打开相机\n";
    cv::VideoCapture capture(0);
    //摄像头是否打开成功
    if (!capture.isOpened())
    {
        std::cout << "[main.cpp::main] 相机打开失败\n";
        return -1;
    }
    //相机参数设置
    CamaraParam camaraParam;
    std::cout << "[main.cpp::main] 正在设置相机参数\n";
    camaraParam.seting(capture);

    //开启串口
    Serial serial;
    if (0 != serial.init())
    {
        std::cout << "[main.cpp::main] 串口初始化异常\n";
        return -1;
    }
    GreenSpot greenSpot;               //目标识别
    DartSocket dartsocket;             //套接字图传
    std::vector<cv::Point> targetData; //用于目标识别返回数据

    //调试窗口
    cv::namedWindow("img", 0);
    cv::namedWindow("bin", 0);

    std::cout << "[main.cpp::main] 相机视频流启动\n";
    while (1)
    {
        capture >> ORIGINMAT;
        if (ORIGINMAT.empty())
        { //判断图像是否为空
            std::cout << "[main.cpp::main] 图像为空\n";
            return -1;
        }
        //dartsocket._accept(); //接受请求处理一帧
        //dartsocket._send(ORIGINMAT); //发送一帧图像
        greenSpot.find(ORIGINMAT);          //目标识别接口
        targetData = greenSpot.getTarget(); //目标识别目标

        if (targetData.size() == 2){ //将数据发给电控
            if (targetData[0].x < 0){
                targetData[0].x = 0;
            }
            if (targetData[1].x > 320){
                targetData[1].x = 320;
            }
            if (targetData[0].y < 0){
                targetData[0].y = 0;
            }
            if (targetData[1].y > 240){
                targetData[1].y = 240;
            }
            int status = serial.send(targetData);//发送串口数据
            if (status == 0){
                std::cout << "[main.cpp::main] 串口数据发送失败\n";
            }
            std::cout << "[main.cpp::main] 数据<<" << targetData[0] << ","
                      << "<<" << targetData[1] << std::endl;
        }
    }
    return 0;
}
