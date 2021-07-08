/*@wanlu 2021-5*/
#include"GreenSpot.h"
#include<stdlib.h>
#include<opencv2/opencv.hpp>
#include<opencv2/ml.hpp>

//----------------------_SaveImage----------------------------
//样本保存器定义
//构造函数
_Dart_SaveImage::_Dart_SaveImage(const char* adress) {
	_saveAdress = new std::string(adress);
	_now = 0;
}
//析构函数
_Dart_SaveImage::~_Dart_SaveImage() {
	delete _saveAdress;
}
//保存
void _Dart_SaveImage::save(cv::Mat& img) {
	if (img.empty())return;
	//将now变为字符串
	std::string filename = std::to_string(_now++);
    if(_now>5000){//文件名大于5000则从零覆盖写入
        _now = 0;
    }
	filename += ".png";
	cv::imwrite(*_saveAdress + filename, img);
}



//---------------------------GreenSpot---------------------------->
//构造函数
GreenSpot::GreenSpot(){
    //删除飞行视角图片
    std::cout<<"delete /var/www/html/Dart/*\n";
    //int back=system("rm -rf /var/www/html/Dart/*");
    //svm绿光团
    std::cout<<"load ../xml/GREEN_SVM_DATA.xml\n";
    _circleGreenSvm== cv::Algorithm::load<cv::ml::SVM>("../xml/GREEN_SVM_DATA.xml");
    //图片保存器初始化
    _saveimg=new _Dart_SaveImage("/var/www/html/Dart/");
    //opencv 3*3 kernel
    _KERNEL=cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
}
GreenSpot::~GreenSpot(){
    delete _saveimg;
}


//送入图像
void GreenSpot::find(cv::Mat&img){
    if(img.empty())return;
    _originMat=img.clone();
    //判断图像是否获取成功
    if(_originMat.empty()){
        std::cout<<"GreenSpot::获取图像失败\n";
        return;
    }
    //图像预处理
    _preprocess();
    _findContours();
    //_SVMfilterRects();
    _featureFilter();
    _findTheBest();

    cv::imshow("bin",_originMat);
    cv::waitKey(1);

    //_saveimg->save(_originMat);

}

//图像预处理
void GreenSpot::_preprocess(){
    //灰度图像
    cv::cvtColor(_originMat,_greyMat,cv::COLOR_BGR2GRAY);
    //通道分离
    _channels.clear();
    cv::split(_originMat,_channels);
    //通道相减
    cv::subtract(_channels[1],_channels[0],_subRed);
    cv::subtract(_channels[1],_channels[2],_subBlue);
    //二值化
    cv::threshold(_subRed,_subRed,55,255,cv::THRESH_BINARY);
    cv::threshold(_subBlue,_subBlue,55,255,cv::THRESH_BINARY);
    cv::threshold(_greyMat,_binaryMat,55,255,cv::THRESH_BINARY);
    _binaryMat=_binaryMat&_subRed&_subBlue;
    //膨胀
    cv::dilate(_binaryMat,_binaryMat,_KERNEL,cv::Point(-1,-1),2);
    //闭操作
    cv::morphologyEx(_binaryMat, _binaryMat, cv::MORPH_CLOSE, _KERNEL);
}


//发现所有轮廓
void GreenSpot::_findContours(){
    //std::cout<<"寻找轮廓\n";
    _contours.clear();
    _hierachy.clear();
    _rotatedRects.clear();
    _contoursIndexs.clear();
    //查找轮廓信息
    cv::findContours(_binaryMat, _contours, _hierachy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
    if (_contours.size() == 0)return;
    //根据轮廓面积过滤一波
    std::vector<std::vector<cv::Point>>::iterator contour = _contours.begin();
    std::vector<cv::Vec4i>::iterator hierachy = _hierachy.begin();
    std::vector<cv::RotatedRect>::iterator rect;
  
    //面积限制
    float minArea=18*18;
    float maxArea=93*93;
    size_t i = 0;
    while (contour != _contours.end() && hierachy != _hierachy.end()) {
	if((*hierachy)[3]!=-1){
		contour++;
		hierachy++;
		i++;
		continue;
	}
        double area = cv::contourArea(*contour);
        //if (area > maxArea || area < minArea) {//面积显示，以及轮廓限制,必须是父轮廓
            //不符合面积条件
       // } else {
            //获得旋转矩形
            cv::RotatedRect tempRect = cv::minAreaRect(*contour);
            //矩形限制
            //bool limit_rect = tempRect.size.width < 18.f || tempRect.size.height < 18.f || tempRect.size.width>93 || tempRect.size.height>93;
            //if (limit_rect==true) {
                //满足基本条件
                _rotatedRects.push_back(tempRect);
                _contoursIndexs.push_back(i);
            //}
        //}
        contour++;
        hierachy++;
        i++;
    }

}

//画旋转矩形
void GreenSpot::_drawRotatedRect(cv::Mat&img,cv::RotatedRect&rect,int b,int g,int r){
	cv::Point2f pts[4];
	rect.points(pts);
	cv::line(img,pts[0],pts[1],cv::Scalar(b,g,r),3,8);
	cv::line(img,pts[1],pts[2],cv::Scalar(b,g,r),3,8);
	cv::line(img,pts[2],pts[3],cv::Scalar(b,g,r),3,8);
	cv::line(img,pts[3],pts[0],cv::Scalar(b,g,r),3,8);
}


//svm绿光团 绿色轮廓
void GreenSpot::_SVMfilterRects(){
    // std::cout<<"SVM 绿光\n";
    // //根据现有的rects过滤一波
    // std::vector<cv::RotatedRect>::iterator rect=_rotatedRects.begin();
    // std::vector<int>::iterator index = _contoursIndexs.begin();
    // while(rect!=_rotatedRects.end()&&index!=_contoursIndexs.end()){
    //     cv::Rect bounding = rect->boundingRect();
    //     //检测获取ROI是否合法
    //     if(_ROIJudge(bounding)==false){
    //         rect = _rotatedRects.erase(rect);
    //         index = _contoursIndexs.erase(index);
    //         continue;
    //     }
    //     //使用svm判断
    //     cv::Mat testMat = _originMat(bounding);
    //     //resize
    //     cv::resize(testMat, testMat, cv::Size(30, 30), 0, 0);
	//     cv::cvtColor(testMat,testMat,CV_BGR2GRAY);
	//     //cv::imshow("bin",testMat);
	//     //cv::waitKey(0);
    //     cv::Mat p = testMat.reshape(1, 1);
    //     p.convertTo(p, CV_32FC1);
    //     cv::normalize(p, p);
    //     //std::cout<<p<<std::endl;
    //     //std::cout<<"predict\n";
    //     int svmJudge=(int)_circleGreenSvm->predict(p);
    //     std::cout<<svmJudge<<std::endl;
	//     //std::cout<<"predicted\n";
    //     if(!svmJudge){
    //         rect = _rotatedRects.erase(rect);
    //         index = _contoursIndexs.erase(index);
    //         continue;
    //     }
    //     rect++;
    //     index++;
    // }
}


//检测获取ROI是否合法:与灰度图像大小比较
bool GreenSpot::_ROIJudge(cv::Rect rect){
    cv::Point tl = rect.tl();
    cv::Point br = rect.br();
    if(tl.x<1||tl.y<1){
        return false;
    }
    if(br.x>_greyMat.cols-1||br.y>_greyMat.rows-1){
        return false;
    }
    return true;
}

//寻找最优目标
void GreenSpot::_findTheBest(){
     for(int i=0;i<_contoursIndexs.size();i++){
         _drawRotatedRect(_originMat,_rotatedRects[_contoursIndexs[i]],255,0,0);
         //std::cout<<"width:"<<_rotatedRects[i].size.width<<"height"<<_rotatedRects[i].size.height<<std::endl;
     }
     _result.clear();
     //在合法轮廓中取出第一个
     if(_contoursIndexs.size()>0){
         cv::Rect bounding=_rotatedRects[_contoursIndexs[0]].boundingRect();
         _result.push_back(bounding.tl());
         _result.push_back(bounding.br());
     }
}


//根据可能的绿球来，检测绿球的偏上位置有没有蓝色，或者红色
//绿球特征过滤
void GreenSpot::_featureFilter(){
    _rects.clear();//清除轮廓矩形(非旋转)
    //遍历轮廓
    std::vector<cv::RotatedRect>::iterator rect = _rotatedRects.begin();
    std::vector<int>::iterator index = _contoursIndexs.begin();
    while(rect!=_rotatedRects.end()&&index!=_contoursIndexs.end()){
        //检测特征的ROI矩形：大小根据绿色球大小来定
        /*
           ||    ||
           ||    ||
            |----|
            |绿  |
            |----|
        向上扩展绿色1.5倍高
        */
        _rects.push_back(rect->boundingRect());
        std::vector<cv::Rect>::iterator last = _rects.end() - 1;
        int height = last->height;
        int width = last->width;
        cv::Point tl(rect->center.x - width * 1.3, rect->center.y - 3.5 * height);
        cv::Point br(rect->center.x + width * 1.3, rect->center.y + 0.6 * height);
        cv::Rect featureROIRect(tl,br);
        //检测ROI是否合法
        if(_ROIJudge(featureROIRect)){
            //截取ROI图像
            cv::Mat ROIMat = _originMat(featureROIRect);
            cv::Mat ROIGrey = _greyMat(featureROIRect);
            cv::Mat ROIBinary;
            std::vector<std::vector<cv::Point>>ROIcontours;//存储轮廓
            std::vector<cv::Vec4i>ROIhierachy;//轮廓拓扑
            //显示图像
            cv::imshow("img",ROIMat);
            cv::waitKey(1);
            //需要对ROI最红蓝二值处理，来判断是否为真正的指示灯
            std::vector<cv::Mat>ROIchannels;
            cv::split(ROIMat,ROIchannels);
            //检测红色--------------------------------------------------------->
            //通道相减0b 1g 2r
            cv::subtract(ROIchannels[2],ROIchannels[1],_subGreen);
            cv::subtract(ROIchannels[2],ROIchannels[0],_subBlue);
            //二值化
            cv::threshold(_subGreen,_subGreen,55,255,cv::THRESH_BINARY);
            cv::threshold(_subBlue,_subBlue,55,255,cv::THRESH_BINARY);
            cv::threshold(ROIGrey,ROIBinary,55,255,cv::THRESH_BINARY);
            ROIBinary=ROIBinary&_subGreen&_subBlue;
            //膨胀
            cv::dilate(ROIBinary,ROIBinary,_KERNEL,cv::Point(-1,-1),2);
            //闭操作
            cv::morphologyEx(ROIBinary, ROIBinary, cv::MORPH_CLOSE, _KERNEL);
	    //cv::imshow("img",ROIBinary);
	    //cv::waitKey(1);
            //寻找轮廓
            cv::findContours(ROIBinary, ROIcontours, ROIhierachy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
            //根据轮廓数量进行判断
            int redContourSize = ROIcontours.size();
            //检测蓝色---------------------------------------------------------->
            //通道相减0b 1g 2r
            cv::subtract(ROIchannels[0],ROIchannels[1],_subGreen);
            cv::subtract(ROIchannels[0],ROIchannels[2],_subRed);
            //二值化
            cv::threshold(_subGreen,_subGreen,55,255,cv::THRESH_BINARY);
            cv::threshold(_subRed,_subRed,55,255,cv::THRESH_BINARY);
            cv::threshold(ROIGrey,ROIBinary,55,255,cv::THRESH_BINARY);
            ROIBinary=ROIBinary&_subGreen&_subRed;
            //膨胀
            cv::dilate(ROIBinary,ROIBinary,_KERNEL,cv::Point(-1,-1),2);
            //闭操作
            cv::morphologyEx(ROIBinary, ROIBinary, cv::MORPH_CLOSE, _KERNEL);
	    //cv::imshow("img",ROIBinary);
	    //cv::waitKey(1);
            //寻找轮廓
            ROIcontours.clear();
            ROIhierachy.clear();
            cv::findContours(ROIBinary, ROIcontours, ROIhierachy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
            //根据轮廓数量进行判断
            int blueContourSize = ROIcontours.size();
            //进行决策去留
            if(redContourSize<1||blueContourSize<1){//不满条件
                rect = _rotatedRects.erase(rect);
                index = _contoursIndexs.erase(index);
                continue;
            }
        }else{//ROI矩形不合法,将不采取 //不满足条件
            rect = _rotatedRects.erase(rect);
            index = _contoursIndexs.erase(index);
            continue;
        }
        rect++;
        index++;
    }
}



//<--------------识别接口--------------->
std::vector<cv::Point> GreenSpot::getTarget(){
    std::vector<cv::Point> result;
    if(_result.size()==2){
        return _result;
    }else{
        return result;//null
    }
}
