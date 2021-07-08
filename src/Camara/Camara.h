/*@wanlu 2021-5*/
#ifndef __CAMARA_H__
#define __CAMARA_H__
#include<opencv2/opencv.hpp>
class CamaraParam{
public:
	static void seting(cv::VideoCapture&capture){
		//设置相机参数
    	capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);//宽度 320
    	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);//高度240
    	capture.set(CV_CAP_PROP_FPS, 90);//帧率 帧/秒30
    	capture.set(CV_CAP_PROP_BRIGHTNESS, 0.40);//亮度 0.5
    	capture.set(CV_CAP_PROP_CONTRAST,0.6);//对比度 0.5
    	capture.set(CV_CAP_PROP_SATURATION, 0.6);//饱和度 0.5
    	//capture.set(CV_CAP_PROP_HUE, 50);//色调 -1
    	//capture.set(CV_CAP_PROP_EXPOSURE, 50);//曝光 inf
		capture.set(CV_CAP_PROP_ISO_SPEED,60000);//ISO
		capture.set(CV_CAP_PROP_GAMMA,2000);//GAMMA
		showdata(capture);
	}
	static void showdata(cv::VideoCapture&capture){
		//输出相机相关信息
    	std::cout<<"宽度:"<<capture.get(CV_CAP_PROP_FRAME_WIDTH)<<std::endl;
    	std::cout<<"高度:"<<capture.get(CV_CAP_PROP_FRAME_HEIGHT)<<std::endl;
    	std::cout<<"帧率:"<<capture.get(CV_CAP_PROP_FPS)<<std::endl;
    	std::cout<<"亮度:"<<capture.get(CV_CAP_PROP_BRIGHTNESS)<<std::endl;
    	std::cout<<"对比度:"<<capture.get(CV_CAP_PROP_CONTRAST)<<std::endl;
    	std::cout<<"饱和度:"<<capture.get(CV_CAP_PROP_SATURATION)<<std::endl;
    	std::cout<<"色调:"<<capture.get(CV_CAP_PROP_HUE)<<std::endl;
    	std::cout<<"曝光:"<<capture.get(CV_CAP_PROP_EXPOSURE)<<std::endl;
    	std::cout<<"ISO_SPEED:"<<capture.get(CV_CAP_PROP_ISO_SPEED)<<std::endl;
    	std::cout<<"GAMMA:"<<capture.get(CV_CAP_PROP_GAMMA)<<std::endl;
	}
};
#endif
