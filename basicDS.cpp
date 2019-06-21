#include "basicDS.h"
#include "utils.h"
using namespace RS;

//----------------------------------
//--------BasicPoint----------------
//----------------------------------
void RS::BasicPoint::reInit(){
    red=0;
    green=0;
    blue=0;
    alpha=0;
}

void RS::BasicPoint::display(){
    printf("Point:%p Red:%d Green: %d Blue: %d Alpha: %d\n",
        this,(uint16_t)red,(uint16_t)green,(uint16_t)blue,(uint16_t)alpha);
}


BasicPoint& RS::BasicPoint::operator=(const BasicPoint& p){
    red=p.red;
    green=p.green;
    blue=p.blue;
    alpha=p.alpha;
    return *this;
}

//----------------------------------
//--------BasicLayer----------------
//----------------------------------

void RS::BasicLayer::reInit(){
    pointMatrix newMatrix(0,std::vector<RS::BasicPoint>(0));
    datamatrix=newMatrix;
    name="";
    layerNumber=0;
    Valid=true;
}

void RS::BasicLayer::reInit(const int16_t width,const int16_t length){
    pointMatrix newMatrix(width,std::vector<RS::BasicPoint>(length));
    datamatrix=newMatrix;
    name="";
    layerNumber=0;
    Valid=true;  
}

void RS::BasicLayer::display(){
    printf("DataMatrix:  %p\n",&datamatrix);
    printf("Name:        %s\n",name.c_str());
    printf("LayerNumber: %d\n",layerNumber);
    printf("Valid:       %d\n",(int16_t)Valid);
    printf("Width:       %zu\n",datamatrix.size());
    printf("Length:      %zu\n",datamatrix.size()==0?0:datamatrix[0].size());
}

RS::BasicLayer& RS::BasicLayer::operator=(const BasicLayer& l){
    datamatrix=l.datamatrix;
    name=l.name;
    layerNumber=l.layerNumber;
    Valid=l.Valid;
    return *this;
}


//----------------------------------
//--------BasicImage----------------
//----------------------------------

void RS::BasicImage::reInit(const uint16_t width=0,const uint16_t length=0){
    nameToIndex.clear();
    RS::BasicLayer newLayer(width,length);
    insert(newLayer);    
}

void RS::BasicImage::display() const{
    printf("Image:      %p\n",this);
    printf("Name:       %s\n",name.c_str());
    printf("ValidLayer: %hhu\n",validLayer);
    printf("TotalLayer: %hhu\n",totalLayer);
    printf("Current:    %hhu\n",current);
    std::cout<<std::endl;
    for(auto dude:layers){
        dude.display();
    }
}

bool RS::BasicImage::insert(const RS::BasicLayer& aLayer){
    layers.push_back(aLayer);
    if(nameToIndex.insert({aLayer.getLayerName(),totalLayer}).second==true){
        ++totalLayer;
        if(aLayer.isValid())++validLayer;
        return true;
    }
    return false;
}

const std::string& RS::BasicImage::getImageName() const {
    return this->name;
}

RS::BasicLayer& RS::BasicImage::getLayer(const uint16_t index){
    if(index>=layers.size()||index<0){
        err("Index error.\n");
    }
    return layers[index];
}

RS::BasicLayer& RS::BasicImage::getLayer(const std::string& name){
    if(!nameToIndex.count(name)){
        err("Index error\n");
    }
    uint16_t index=nameToIndex[name];
    return RS::BasicImage::getLayer(index);
}

RS::BasicLayer& RS::BasicImage::getCurrentLayer(){
    return RS::BasicImage::getLayer(current);
}

bool RS::BasicImage::remove(const uint16_t index){
    if(index>=layers.size()||index<0){
        err("Index error.\n");
        return false;
    }
    layers.erase(layers.begin()+index);
    return true;
}

bool RS::BasicImage::remove(const std::string& name){
    if(!nameToIndex.count(name)){
        err("Index error\n");
    }
    uint16_t index=nameToIndex[name];
    RS::BasicImage::remove(index);
}

