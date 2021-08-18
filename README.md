# 飞镖视觉
## 题目总览
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;飞镖视觉我认为是整赛季开发最坎坷的项目、硬件平台用什么合适?软件用什么开发？我们到底要做什么?有用吗?这是我刚接触它时的问题。
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;在飞镖机身上使用视觉方案不一定能够解决问题、难度较大、没有存在理论支持的视觉解决方案。可能要不断的迭代发展、各位机甲大师们能够有大的突破。视觉方案应该装载飞镖发射架上还是飞镖机身上呢。虽然本赛季装载在机身上，一个赛季依赖我认为装载在飞镖架上使用MiniPC与地面兵种同样解决方案比较靠谱。但缺少可行性验证，这只是在现在装载在机身角度来看。在我接触项目前后，都没有使用过OpenMV平台，如果装载在机身上的视觉平台我现在认为OpenMV是最好的。经过赛季的验证树莓派Zero是不足以支撑庞大项目快速运行的。
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;首先硬件平台往后若要改进，机身建议有树莓派CM4模块+定制mini载板或OpenMV。发射架建议与地面兵种同样配置。
## 本赛季存在问题及建议
### 赛前
* 项目建立较晚、由于种种原因
* 寻找硬件平台与相机选择上花费较长时间
### 赛中
* 由于电路控制与视觉没有找到有效的飞镖视觉控制方案只能由舍弃
## 题目分析
#### 目标
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;找到基地或前哨战上方的绿色圆灯，将数据发送给电控
#### 解决方案
##### Github仓库地址
https://github.com/gaowanlu/RMDartsVision
##### 实验室Gitlib地址
http://git.peterli.club/gao.wanlu/dart2021
##### 关于绿灯的识别
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;简单粗暴、直接二值图找轮廓判断大小进行判断。【可增强之处：绿色登上方有装甲板、这也是其重要特征，因为要击中装甲板才是击中目标】

##### 串口通信
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;串口通信，需要收发同时，则要使用多线程来解决问题，项目中由于原因没有收发校验以及CRC校验等，开发者如果参考此项目可进行改进添加、修正错误。
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;此项目将发现到的绿色灯的范围矩形的四个在二位图像坐标发送至电控，有电控做出处理。
##### 树莓派环境配置及串口设置
* 请看底部补充部分

## 项目工程文档
### DART2021

### 项目简介
`桂林电子科技大学 RoboMaster2021赛季 飞镖视觉`

#### 开源协议 MIT

#### 项目环境
|  工具   | 依赖库  | 硬件 | 系统 |  
|  ----  | ----  | ----|----|
| g++  | pthread |树莓派Zero W  |Raspbian-buster |
|  make | opencv3 |  | |
|cmake | | | |


#### 视觉项目贡献
________________________________
高万禄|张喆宇|张李昱|21赛季的队友们 


#### 项目配置注意事项
##### 关于Opencv3环境解决方案
```bash
$sudo apt-get update
$sudo apt-get remove libopencv-dev
$sudo apt-get autoremove
$sudo apt-get install libopencv-dev
$sudo ldconfig
```
#### 项目构建
```bash
$cmake .
$make
```

#### 运行项目
```bash
$sudo ./bin/Dart
```

#### 树莓派串口配置 /dev/ttyAMA0
```bash
$sudo vim /boot/config.txt    
讲一下两行加到  /boot/config.txt 内容下面  
enable_uart=1   
dtoverlay=pi3-disable-bt  

$sudo systemctl stop serial-getty@ttyAMA0.service  
$sudo systemctl disable serial-getty@ttyAMA0.service  

$sudo vim /boot/cmdline.txt  
delete console content...  

$sudo reboot
```


#### 开机自启设置
```bash
$ sudo vim /etc/rc.local  
sleep 5  
cd /SD/dart2021/bin  
sudo ./Dart&  
加到exit 0前  
```


#### 项目文件
* bin(生成可执行文件目录)
* login(桂林电子科技大学机器人中心校园网认证)  
* xml(XML文件目录)  
* client(C SOCKET 图像传输接收端)  
* src(项目源码)  
|---main.cpp(主函数)  
  |---GreenSpot(图像目标识别)  
  &emsp;&emsp;|--GreenSpot.cpp  
  &emsp;&emsp;|--GreenSpot.h  
  |---Serial(串口通信)  
  &emsp;&emsp;|---SerialTool.cpp(Linux串口通信工具)  
  &emsp;&emsp;|---SerialTool.h  
  &emsp;&emsp;|---SerialPack.cpp(通信协议解包封包)  
  &emsp;&emsp;|---SerialPack.h  
  |---Camara(相机参数设置)  
  &emsp;&emsp;|---Camara.h  
  |---DartSocket(Socket图像传输发送端)  
  &emsp;&emsp;|---DartSocket.cpp  
  &emsp;&emsp;|---DartSocket.h  

#### 附加功能
* TCP图像传输
* 登录机器人中心校园网认证[在Linux命令行下使用login程序]





