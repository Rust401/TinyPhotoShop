#ifndef BASIC_DS_H
#define BASIC_DS_H

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#define pointMatrix std::vector<std::vector<RS::BasicPoint>>
#define dataBuffer std::vector<std::vector<uint32_t>>

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
    
    virtual uint8_t getRed() const{return (uint16_t)red;}
    virtual uint8_t getGreen() const{return (uint16_t)green;}
    virtual uint8_t getBlue() const{return (uint16_t)blue;}
    virtual uint8_t getAlpha() const{return (uint16_t)alpha;}

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
    std::string name;
    uint16_t layerNumber;
    bool Valid;
public:
    BasicLayer(){reInit();}
    BasicLayer(const int16_t width,const int16_t length){reInit(width,length);}
    BasicLayer(const pointMatrix& datamatrix,const std::string name,
        const uint16_t layerNumber,const bool Valid):datamatrix(datamatrix),
        name(name),layerNumber(layerNumber),Valid(Valid){}
    virtual ~BasicLayer(){}
    virtual void reInit();
    virtual void reInit(const int16_t width,const int16_t length);
    virtual void display();

    virtual const pointMatrix& getDataMatrix() const {return datamatrix;}
    virtual pointMatrix& getDataMatrix() {return datamatrix;}
    virtual const std::string& getLayerName() const {return name;}
    virtual uint16_t getLayerNumber() const {return layerNumber;}
    virtual bool isValid() const {return Valid;}

    virtual void setDataMatrix(const pointMatrix& matrix){datamatrix=matrix;}
    virtual void setDataMatrix(const std::vector<std::vector<uint32_t>>& buffer){
        //TO DO
    }
    virtual void setLayerName(const std::string& name){this->name=name;}
    virtual void setLayerNumber(const uint16_t number){layerNumber=number;}
    virtual void setValid(){Valid=true;}
    virtual void setInvalid(){Valid=false;}

    BasicLayer& operator=(const BasicLayer& layer);
};

class BasicImage
{
protected:
    std::vector<RS::BasicLayer> layers;
    std::string name;
    std::unordered_map<std::string,uint16_t> nameToIndex;
    uint16_t validLayer;
    uint16_t totalLayer;
    uint16_t current;
    bool indexOK(const uint16_t index){return index>=0&&index<layers.size();}
public:
    BasicImage(){}

    BasicImage(const BasicLayer& aLayer,const std::string& name="default"):
        name(name){
        uint16_t currentIndex=0;
        validLayer=0;
        totalLayer=0;
        current=0;
        insert(aLayer);
    }

    BasicImage(const uint16_t width,const uint16_t length,
        const std::string& name="default"){
        uint16_t currentIndex=0;
        validLayer=0;
        totalLayer=0;
        current=0;
        RS::BasicLayer aLayer(width,length);
        insert(aLayer);
    }
    ~BasicImage(){}

    
    virtual void reInit(const uint16_t width=0,const uint16_t length=0);
    virtual void display() const;

    virtual const std::string& getImageName() const;
    virtual RS::BasicLayer& getLayer(const std::string& name);
    virtual RS::BasicLayer& getLayer(const uint16_t index);
    virtual RS::BasicLayer& getCurrentLayer();

    virtual bool insert(const RS::BasicLayer& aLayer);
    virtual bool insert(std::string name,dataBuffer& buffer){/* TODO */}
    virtual bool remove(const uint16_t index);
    virtual bool remove(const std::string& name);
    virtual bool duplicate(const uint16_t index);
    virtual bool duplicate(const std::string& name);
    virtual bool swap(const uint16_t index1,const uint16_t index2);
    virtual bool swap(const std::string& name1,const std::string& name2);
    virtual bool updateLayer(const std::string& name,const dataBuffer& buffer){/* TODO */}
    virtual bool updateLayer(const uint16_t index,const dataBuffer& buffer){/* TODO */}
};
}

#endif //BASIC_DS_H