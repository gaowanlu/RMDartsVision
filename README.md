# DART2021
## 项目简介
`桂林电子科技大学 RoboMaster2021赛季 飞镖视觉`
### 项目环境
* g++
* linux
* 树莓派zero W
* make
* cmake

### 视觉项目贡献
________________________________
   
--------------------------------

## 新下载项目事项
* 关于Opencv环境解决方案
```bash
* method-1
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

### 关于串口配置
$sudo vim /boot/config.txt  
add content... 
enable_uart=1 
dtoverlay=pi3-disable-bt  

$sudo systemctl stop serial-getty@ttyAMA0.service  
$sudo systemctl disable serial-getty@ttyAMA0.service  

$sudo vim /boot/cmdline.txt  
delete console content...  

$sudo reboot 


### 开机自启设置
$ sudo vim /etc/rc.local  
sleep 5  
cd /SD/dart2021/bin  
sudo ./Dart&  
加到exit 0前  



## 项目文件
* bin(可执行文件目录)
* login(robot center wifi)  
* xml(ml xml)  
* client(socket video client)  
* src(项目源码)  
|---main.cpp(主函数)  
  |---GreenSpot(绿光团识别)  
  &emsp;&emsp;|--GreenSpot.cpp    
  &emsp;&emsp;|--GreenSpot.h  
  |---Serial(串口通信)  
  &emsp;&emsp;|---Serial.cpp  
  &emsp;&emsp;|---Serial.h  
  |---Camara  
  &emsp;&emsp;|---Camara.h  
  |---DartSocket  
  &emsp;&emsp;|---DartSocket.cpp  
  &emsp;&emsp;|---DartSocket.h  


