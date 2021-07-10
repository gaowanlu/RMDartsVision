# DART2021

## 项目简介
`桂林电子科技大学 RoboMaster2021赛季 飞镖视觉`

### 开源协议 MIT

### 项目环境
|  工具   | 依赖库  | 硬件 | 系统 |  
|  ----  | ----  | ----|----|
| g++  | pthread |树莓派Zero W  |Raspbian-buster |
|  make | opencv3 |  | |
|cmake | | | |


### 视觉项目贡献
________________________________
`高万禄` `张喆宇` `张李昱` ` 21赛季的队友们`  
--------------------------------

## 项目配置注意事项
### 关于Opencv3环境解决方案
```bash
* 方法1
$sudo apt-get update
$sudo apt-get remove libopencv-dev
$sudo apt-get autoremove
$sudo apt-get install libopencv-dev
$sudo ldconfig
```
### 项目构建
```bash
$cmake .
$make
```

### 运行项目
```bash
$sudo ./bin/Dart
```

### 树莓派串口配置 /dev/ttyAMA0
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


### 开机自启设置
```bash
$ sudo vim /etc/rc.local  
sleep 5  
cd /SD/dart2021/bin  
sudo ./Dart&  
加到exit 0前  
```


## 项目文件
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
