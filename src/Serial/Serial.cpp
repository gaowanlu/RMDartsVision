#include"Serial.h"
#include<opencv2/opencv.hpp>
#include<vector>
Serial::Serial(){
    //打开串口
    _serial=open("/dev/ttyAMA0",O_RDWR|O_NOCTTY|O_NDELAY);
    if(_serial==-1){	
        //@
	    std::cout<<"Serial::串口打开失败\n"<<std::endl;
	    exit(0);
    }
}
Serial::~Serial(){
    close(_serial);//关闭串口
}
int Serial::init(){
    //恢复阻塞状态
	if(0>fcntl(_serial,F_SETFL,0)){
        //@
		printf("Serial::恢复阻塞状态失败\n");
        return -1;
    }
    //绑定串口状态
	tcgetattr(_serial,&_opt);
	//波特率设置
	cfsetispeed(&_opt, B115200);
	cfsetospeed(&_opt, B115200);
    if(tcsetattr(_serial,TCSANOW,&_opt)!=0)
	{
		//@
		std::cout<<"Serial::tcsetattr()错误\n"<<std::endl;
		return -1;
	}
    _opt.c_cflag &=~CSIZE;
	_opt.c_cflag |=CS8;
	_opt.c_cflag &=~CSTOPB;
	_opt.c_cflag &=~PARENB;
	_opt.c_iflag &=~INPCK;
	_opt.c_cflag |=(CLOCAL|CREAD);
	_opt.c_iflag &=~(ICANON|ECHO|ECHOE|ISIG);
	_opt.c_oflag &=~OPOST;
	_opt.c_oflag &=~(ONOCR|OCRNL);
	_opt.c_iflag &=~(ICRNL|INLCR);
	_opt.c_iflag &=~(IXON|IXOFF|IXANY);
	_opt.c_cc[VMIN]=0;
    //清空缓冲区
    tcflush(_serial,TCIOFLUSH);
    //@
	std::cout<<"Serial::	串口配置完毕\n"<<std::endl;
    if(tcsetattr(_serial,TCSANOW,&_opt)!=0)
	{
		//@
		std::cout<<"Serial::	tcsetattr 处发生错误\n"<<std::endl;
		return -1;
	}
	//@
	std::cout<<"Serial::	一切准备就绪\n"<<std::endl;
    return 0;
}
/*
向嵌入式串口发送数据,只发目标的(x,y)坐标
图像的大小在嵌入式固定即可
*/
int Serial::send(std::vector<cv::Point>result){
    if(result.size()!=2) return 0;
    static int x[2],y[2];
    x[0]=result[0].x;y[0]=result[0].y;
    x[1]=result[1].x;y[1]=result[1].y;
    //帧头
    _buffer[0] = BEFORE;
    _buffer[1] = AFTER;
    _buffer[2] = NUMBER;
    //数据
    _buffer[3]=x[0]/256;
    _buffer[4]=x[0]%256;
    _buffer[5]=y[0]/256;
    _buffer[6]=y[0]%256;
    _buffer[7]=x[1]/256;
    _buffer[8]=x[1]%256;
    _buffer[9]=y[1]/256;
    _buffer[10]=y[1]%256;
    //发送
    static int len=-1;
    len=write(_serial,_buffer,11);
    if(len==11){//发送成功
        return 1;
    }else{//发送错误
        return 0;
    }
}
