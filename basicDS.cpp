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

uint32_t RS::BasicPoint::getUint32() const{
    uint32_t result;
    uint8_t* p=(uint8_t*)&result;
    *p=getRed();
    *(p+1)=getGreen();
    *(p+2)=getBlue();
    *(p+3)=getAlpha();
    return result;
}


//----------------------------------
//--------BasicLayer----------------
//----------------------------------
RS::BasicLayer::BasicLayer(const dataBuffer& data){
    reInit(data);
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

void RS::BasicLayer::reInit(const dataBuffer& data){
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

void RS::BasicLayer::display(){
    printf("DataMatrix:  %p\n",&datamatrix);
    printf("Name:        %s\n",name.c_str());
    printf("LayerNumber: %d\n",layerNumber);
    printf("Valid:       %d\n",(int16_t)Valid);
    printf("Width:       %zu\n",datamatrix.size());
    printf("Length:      %zu\n",datamatrix.size()==0?0:datamatrix[0].size());
}

void RS::BasicLayer::displayData(){
    if(datamatrix.size()==0){std::cout<<"null"<<std::endl;return;}
    if(datamatrix[0].size()==0){std::cout<<"null"<<std::endl;return;}
    for(int i=0;i<datamatrix.size();++i){
        for(int j=0;j<datamatrix[0].size();++j){
            RS::BasicPoint tmp=datamatrix[i][j];
            printf("%x %x %x %x   ",tmp.getRed(),tmp.getGreen(),tmp.getBlue(),
                tmp.getAlpha());
        }
        std::cout<<std::endl;
    }
}

void RS::BasicLayer::setDataMatrix(const dataBuffer& data){
    uint16_t newWidth,newLength;
    if(data.size()==0||data[0].size()==0){
        newWidth=0;
        newLength=0;
    }else{
        newWidth=data.size();
        newLength=data[0].size();
    }
    pointMatrix newDataMatrix(newWidth,std::vector<RS::BasicPoint>(newLength));
    for(int i=0;i<newWidth;++i){
        for(int j=0;j<newLength;++j){
            newDataMatrix[i][j].reInit(data[i][j]);
        }
    }
    datamatrix=newDataMatrix;
}

RS::BasicLayer& RS::BasicLayer::operator=(const BasicLayer& l){
    datamatrix=l.datamatrix;
    name=l.name;
    layerNumber=l.layerNumber;
    Valid=l.Valid;
    return *this;
}

void RS::BasicLayer::rightRotate(){

}

void RS::BasicLayer::leftRotate(){

}

void RS::BasicLayer::upDownReverse(){

}

void RS::BasicLayer::leftRightReverse(){

}

bool RS::BasicLayer::taylor(uint16_t rowS,uint16_t columnS,uint16_t rowE,uint16_t columnE){
    if(datamatrix.size()==0)return false;
    if(datamatrix[0].size()==0)return false;
    if(rowS<0||rowS>=datamatrix.size())return false;
    if(columnS<0||columnS>datamatrix[0].size())return false;
    if(rowE<0||rowE>=datamatrix.size()||rowE<=rowS)return false;
    if(columnE<0||columnE>=datamatrix[0].size()||columnE<=columnS)return false;
    uint16_t newWidth=rowE-rowS+1;
    uint16_t newLength=columnE-columnS+1;
    dataBuffer tmp(newWidth,rowData(newLength,0));
    for(int i=rowS;i<rowE;++i){
        for(int j=columnS;j<columnS;++j){
            tmp[i][j]=datamatrix[i][j].getUint32();
        }
    }
    std::string newName=name;
    bool newValid=isValid();
    reInit(tmp);
    name=newName;
    Valid=newValid;
    return true;
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

bool RS::BasicImage::insert(const std::string& name,const dataBuffer& buffer){
    RS::BasicLayer newLayer(buffer);
    newLayer.setLayerName(name);
    if(insert(newLayer))return true;
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

bool RS::BasicImage::updateLayer(const uint16_t index,const dataBuffer& buffer){
    if(index>=layers.size()||index<0){
        err("Index error.\n");
        return false;
    }
    RS::BasicLayer newLayer(buffer);
    std::string tmpName=layers[index].getLayerName();
    bool tmpValid=layers[index].isValid();
    newLayer.setLayerName(tmpName);
    if(tmpValid)newLayer.setValid();
    else newLayer.setInvalid();
    layers[index]=newLayer;
    return true;
}

bool RS::BasicImage::updateLayer(const std::string& name,const dataBuffer& buffer){
    if(!nameToIndex.count(name)){
        err("No such layer\n");
        return false;
    }
    uint16_t index=nameToIndex[name];
    if(updateLayer(index,buffer))return true;
    return false;
}

bool RS::BasicImage::taylor(const std::string& name,const std::vector<uint16_t>& array){
    if(!nameToIndex.count(name)){
        err("No such layer\n");
        return false;
    }
    uint16_t index=nameToIndex[name];
    if(taylor(index,array))return true;
    return false;
}
bool RS::BasicImage::taylor(const uint16_t index,const std::vector<uint16_t>& array){
    if(index>=layers.size()||index<0){
        err("Index error.\n");
        return false;
    }
    if(layers[index].taylor(array[0],array[1],array[2],array[3]))return true;
    return false;
}

void RS::BasicImage::mergeLayer(const std::string& name1,const std::string& name2){

}
void RS::BasicImage::mergeLayer(const uint16_t index1,const uint16_t index2){
    
}





