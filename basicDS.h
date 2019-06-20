#ifndef BASIC_DS_H
#define BASIC_DS_H

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <memory>

#define pointMatrix std::vector<std::vector<BasicPoint>>

namespace RS
{
class BasicPoint
{
protected:
    uint8_t red,green,blue,alpha;
public:
    BasicPoint(){red=0;green=0;blue=0;alpha=0;}
    BasicPoint(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha=0):
        red(red),green(green),blue(blue),alpha(alpha){}
    virtual ~BasicPoint(){};
    virtual void reInit();
    virtual void display();
    
    virtual uint8_t getRed() const{return red;}
    virtual uint8_t getGreen() const{return green;}
    virtual uint8_t getBlue() const{return blue;}
    virtual uint8_t getAlpha() const{return alpha;}

    virtual void setRed(const uint8_t red){this->red=red;}
    virtual void setGreen(const uint8_t green){this->green=green;}
    virtual void setBlue(const uint8_t blue){this->blue=blue;}
    virtual void setAlpha(const uint8_t alpha){this->alpha=alpha;}

    BasicPoint& operator=(const BasicPoint& p);
};

class BasicLayer
{
protected:
    pointMatrix datamatrix;
    bool isValid;
};


}


#endif //BASIC_DS_H