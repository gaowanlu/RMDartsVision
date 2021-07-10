#include"SerialPack.h"
SerialToolPack::SerialToolPack(){
    x1=0;
    x2=0;
    y1=0;
    y2=0;
}

void SerialToolPack::unpack(unsigned char*buffer,int size){
    if(size!=11){
      return;
    }
    x1=(buffer[3]<<8);
    x1=x1 | buffer[4];
    
    y1=(buffer[5]<<8);
    y1=y1 | buffer[6];
    
    x2=(buffer[7]<<8);
    x2=x2 | buffer[8];
    
    y2=(buffer[9]<<8);
    y2=y2 | buffer[10];
}

void SerialToolPack::pack(unsigned char*buffer,int size){
    if(size!=11){
        return;
    }
    buffer[0]=0xAA;
    buffer[1]=0xAF;
    buffer[2]=0x08;
    
    buffer[3]=(x1>>8)&0xFF;
    buffer[4]=x1 & 0xFF;
    
    buffer[5]=(y1>>8)&0xFF;
    buffer[6]=y1 & 0xFF;
    
    buffer[7]=(x2>>8)&0xFF;
    buffer[8]=x2 & 0xFF;
    
    buffer[9]=(y2>>8)&0xFF;
    buffer[10]=y2 & 0xFF;
}
