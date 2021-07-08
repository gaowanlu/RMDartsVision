/*@wanlu 2021-5*/
#ifndef __GREENSPOT_H__
#define __GREENSPOT_H__
#include<opencv2/opencv.hpp>
#include<vector>
#include<iostream>
#include<opencv2/ml.hpp>
class _Dart_SaveImage {
private:
	long int _now = 0;//记录现在的文件名称
	std::string* _saveAdress;
public:
	//构造函数
	_Dart_SaveImage(const char* adress);
	//析构函数
	~_Dart_SaveImage();
	//保存
	void save(cv::Mat& img);
};


class GreenSpot{
public:
    GreenSpot();
    ~GreenSpot();
    void find(cv::Mat&img);
    std::vector<cv::Point> getTarget();
private:
   //图片保存
    _Dart_SaveImage* _saveimg;
    cv::Mat _originMat;
    cv::Mat _greyMat;
    cv::Mat _binaryMat;
    cv::Mat _subBlue;
    cv::Mat _subRed;
    cv::Mat _subGreen;
    cv::Mat _KERNEL;
    std::vector<std::vector<cv::Point>>_contours;//存储轮廓
    std::vector<cv::Vec4i>_hierachy;//轮廓拓扑
    std::vector<cv::RotatedRect>_rotatedRects;//存储轮廓最小包围旋转矩形
    std::vector<cv::Rect> _rects;
    std::vector<cv::Point>_result;//存储目标
    std::vector<int> _contoursIndexs;//存储可能轮廓的下标
    void _featureFilter();//绿球特征过滤
    void _preprocess();
    void _findContours();
    void _findTheBest();
    std::vector<cv::Mat>_channels;
    void _drawRotatedRect(cv::Mat&img,cv::RotatedRect&rect,int b,int g,int r);
    cv::Ptr<cv::ml::SVM> _circleGreenSvm;
    //svm绿光团 绿色轮廓
    void _SVMfilterRects();
    //检测获取ROI是否合法
    bool _ROIJudge(cv::Rect rect);
};
#endif
