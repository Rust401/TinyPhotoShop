#include "basicDS.h"
#include "utils.h"
using namespace RS;

//----------------------------------
//--------BasicPoint----------------
//----------------------------------
RS::BasicPoint::BasicPoint(const uint32_t data)
{
    //Don't use the memset to initial
    //this class have virtual function!!
    reInit(data);
}

void RS::BasicPoint::reInit(){
    red=0;
    green=0;
    blue=0;
    alpha=0;
}

void RS::BasicPoint::reInit(const uint32_t data){
    uint8_t* p=(uint8_t*)&data;
    red=*p;green=*(p+1);blue=*(p+2);alpha=*(p+3);
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
RS::BasicLayer::BasicLayer(const dataBuffer& data){
    if(data.size()==0)reInit();
    if(data[0].size()==0)reInit();
    uint16_t width=data.size();
    uint16_t length=data[0].size();
    reInit(width,length);
    //use the buffer we get to init our layer
    for(int i=0;i<width;++i){
        for(int j=0;j<length;++j){
            datamatrix[i][j].reInit(data[i][j]);
        }
    }
}

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

void RS::BasicLayer::displayData(){
    if(datamatrix.size()==0)std::cout<<"null"<<std::endl;
    if(datamatrix[0].size()==0)std::cout<<"null"<<std::endl;
    for(int i=0;i<datamatrix.size();++i){
        for(int j=0;j<datamatrix[0].size();++j){
            RS::BasicPoint tmp=datamatrix[i][j];
            printf("%x %x %x %x   ",tmp.getRed(),tmp.getGreen(),tmp.getBlue(),
                tmp.getAlpha());
        }
        std::cout<<std::endl;
    }
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

void RS::BasicImage::reInit(const uint16_t width,const uint16_t length){
    nameToIndex.clear();
    RS::BasicLayer newLayer(width,length);
    insert(newLayer);    
}

void RS::BasicImage::display() const{
    printf("Image:      %p\n",this);
    printf("Name:       %s\n",name.c_str());
    printf("ValidLayer: %hu\n",validLayer);
    printf("TotalLayer: %hu\n",totalLayer);
    printf("Current:    %hu\n",current);
    std::cout<<std::endl;
    for(auto dude:layers){
        dude.display();
        std::cout<<std::endl;
    }
}

void RS::BasicImage::displayHash() const{
    for(auto it=nameToIndex.begin();it!=nameToIndex.end();++it){
        std::cout<<it->first<<" "<<it->second<<std::endl;
    }
}

bool RS::BasicImage::insert(const RS::BasicLayer& aLayer){
    if(nameToIndex.insert({aLayer.getLayerName(),totalLayer}).second==true){
        layers.push_back(aLayer);
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
        err("No such layer\n");
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
    //remove the name-index pair from the hash_map
    for(auto i=nameToIndex.begin();i!=nameToIndex.end();++i){
        if(i->second==index)nameToIndex.erase(i->first);
    }
    layers.erase(layers.begin()+index);
    return true;
}

bool RS::BasicImage::remove(const std::string& name){
    if(!nameToIndex.count(name)){
        err("No such layer\n");
        return false;
    }
    uint16_t index=nameToIndex[name];
    RS::BasicImage::remove(index);
    return true;
}

bool RS::BasicImage::duplicate(const uint16_t index){
    if(index>=layers.size()||index<0){
        err("Index error.\n");
        return false;
    }
    RS::BasicLayer newDude=layers[index];
    newDude.setLayerName(newDude.getLayerName()+"_dup");
    insert(newDude);
    return true;
}

bool RS::BasicImage::duplicate(const std::string& name){
    if(!nameToIndex.count(name)){
        err("No such layer\n");
        return false;
    }
    uint16_t index=nameToIndex[name];
    return RS::BasicImage::duplicate(index);
}

bool RS::BasicImage::swap(const uint16_t index1,const uint16_t index2){
    if(indexOK(index1)&&indexOK(index2)){
        //swap the name-index hash_map uing this stupid way
        for(auto i=nameToIndex.begin();i!=nameToIndex.end();++i){
            if(i->second==index1){i->second=index2;continue;}
            if(i->second==index2){i->second=index1;continue;}
        }
        std::swap(layers[index1],layers[index2]);
        return true;
    }else{
        err("Index error.\n");
        return false;
    }
}

bool RS::BasicImage::swap(const std::string& name1,const std::string& name2){
    if(!nameToIndex.count(name1)||!nameToIndex.count(name2)){
        err("Index errror.\n");
        return false;
    }
    return RS::BasicImage::swap(nameToIndex[name1],nameToIndex[name2]);
}





