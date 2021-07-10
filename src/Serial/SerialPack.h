#ifndef __SERIALPACK_H__
#define __SERIALPACK_H__
class SerialToolPack{
public:
    SerialToolPack();
    unsigned int x1;
    unsigned int y1;
    unsigned int x2;
    unsigned int y2;
    void unpack(unsigned char*buffer,int size);
    void pack(unsigned char*buffer,int size);
};
#endif
