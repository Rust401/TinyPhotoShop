#ifndef BASIC_DS_H
#define BASIC_DS_H

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <memory>
#include <memory.h>
#include <unordered_map>
#include <math.h>

#define pointMatrix std::vector<std::vector<RS::BasicPoint>>
#define dataBuffer std::vector<std::vector<uint32_t>>
#define rowData std::vector<uint32_t>

enum blendMode {
    CLEAR,SRC,DST,SRC_OVER,DST_OVER,SRC_IN,SRC_OUT,DST_IN,DST_OUT,SRC_ATOP,DST_ATOP,XOR
};

namespace RS
{
class BasicPoint
{
protected:
    uint8_t red,green,blue,alpha;
public:
    BasicPoint(){red=0;green=0;blue=0;alpha=0;}
    BasicPoint(const uint8_t red, const uint8_t green, const uint8_t blue,
        const uint8_t alpha=0):red(red),green(green),blue(blue),alpha(alpha){}
    BasicPoint(const uint32_t data);//use a uint32 to initial
    virtual ~BasicPoint(){};
    virtual void reInit();
    virtual void reInit(const uint32_t data);
    virtual void display();
    
    virtual uint8_t getRed() const{return (uint16_t)red;}
    virtual uint8_t getGreen() const{return (uint16_t)green;}
    virtual uint8_t getBlue() const{return (uint16_t)blue;}
    virtual uint8_t getAlpha() const{return (uint16_t)alpha;}
    virtual uint32_t getUint32() const;

    virtual void setRed(const uint8_t red){this->red=red;}
    virtual void setGreen(const uint8_t green){this->green=green;}
    virtual void setBlue(const uint8_t blue){this->blue=blue;}
    virtual void setAlpha(const uint8_t alpha){this->alpha=alpha;}

    virtual std::pair<double,double> getFactor(blendMode mode,double As,double Ad);
    BasicPoint& blend(const BasicPoint& dst,blendMode mode);

    BasicPoint& operator=(const BasicPoint& p);
};

class BasicLayer
{
protected:
    pointMatrix datamatrix;
    std::string name;
    uint16_t layerNumber;//this layerNumber seem to be useless, change it to static?
    bool Valid;
public:
    BasicLayer(){reInit();}
    BasicLayer(const int16_t width,const int16_t length){reInit(width,length);}
    BasicLayer(const pointMatrix& datamatrix,const std::string name,
        const uint16_t layerNumber,const bool Valid):datamatrix(datamatrix),
        name(name),layerNumber(layerNumber),Valid(Valid){}
    BasicLayer(const dataBuffer& datas);
    virtual ~BasicLayer(){}
    virtual void reInit();
    virtual void reInit(const int16_t width,const int16_t length);
    virtual void reInit(const dataBuffer& datas);
    virtual void display();
    virtual void displayData();

    virtual const pointMatrix& getDataMatrix() const {return datamatrix;}
    virtual pointMatrix& getDataMatrix() {return datamatrix;}
    virtual void getDataBuffer(dataBuffer& result) const;
    virtual const std::string& getLayerName() const {return name;}
    virtual uint16_t getLayerNumber() const {return layerNumber;}
    virtual uint16_t getWidth() const;
    virtual uint16_t getLength() const;
    virtual bool isValid() const {return Valid;}
    virtual bool haveSize() const;

    virtual void setDataMatrix(const pointMatrix& matrix){datamatrix=matrix;}
    virtual void setDataMatrix(const std::vector<std::vector<uint32_t>>& buffer);
    virtual void setLayerName(const std::string& name){this->name=name;}
    virtual void setLayerNumber(const uint16_t number){layerNumber=number;}
    virtual void setValid(){Valid=true;}
    virtual void setInvalid(){Valid=false;}

    virtual bool rightRotate();
    virtual bool leftRotate();
    virtual bool upDownReverse();
    virtual bool leftRightReverse();
    virtual bool taylor(uint16_t rowS,uint16_t columnS,uint16_t rowE,uint16_t columnE);

    virtual bool squareRotate();
    virtual bool rectangleRotate();
    
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
public:
    BasicImage();
    BasicImage(const BasicLayer& aLayer,const std::string& name="default");
    BasicImage(const uint16_t width,const uint16_t length, const std::string& name="default");
    BasicImage(const dataBuffer& data);
    virtual ~BasicImage(){}

    virtual void reInit(const uint16_t width=0,const uint16_t length=0);
    virtual void display() const;
    virtual void displayHash() const;
    virtual bool reHash();

    virtual const std::string& getImageName() const;
    virtual RS::BasicLayer& getLayer(const std::string& name);
    virtual RS::BasicLayer& getLayer(const uint16_t index);
    virtual RS::BasicLayer& getCurrentLayer();

    virtual bool insert(const RS::BasicLayer& aLayer);
    virtual bool insert(const std::string& name,const dataBuffer& buffer);
    virtual bool remove(const uint16_t index);
    virtual bool remove(const std::string& name);
    virtual bool duplicate(const uint16_t index);
    virtual bool duplicate(const std::string& name);
    virtual bool swap(const uint16_t index1,const uint16_t index2);
    virtual bool swap(const std::string& name1,const std::string& name2);
    virtual bool updateLayer(const std::string& name,const dataBuffer& buffer);
    virtual bool updateLayer(const uint16_t index,const dataBuffer& buffer);

    virtual bool taylor(const std::string& name,const std::vector<uint16_t>& array);
    virtual bool taylor(const uint16_t index,const std::vector<uint16_t>& array);

    virtual bool mergeLayer(const std::string& name1,const std::string& name2,blendMode mode);
    virtual bool mergeLayer(const uint16_t index1,const uint16_t index2,blendMode mode);
private:
    virtual bool mergeLayerCore(const uint16_t index1,const uint16_t index2,blendMode mode);
    virtual bool checkFit(const uint16_t index1,const uint16_t index2);
    virtual bool indexOK(const uint16_t index);
    virtual int16_t findByName(const std::string& name);

};
}

#endif //BASIC_DS_H