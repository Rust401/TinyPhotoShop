#include "basicDS.h"
#include "utils.h"
#include <cmath>
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

RS::BasicPoint::BasicPoint(const BasicPoint& p)
{
    this->red=p.getRed();
    this->green=p.getGreen();
    this->blue=p.getBlue();
    this->alpha=p.getAlpha();
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

BasicPoint& RS::BasicPoint::blend(const BasicPoint& dst,blendMode mode){
    uint8_t AsInt=this->alpha;
    uint8_t AdInt=dst.alpha;
    double As=AsInt/255.0;
    double Ad=AdInt/255.0;
    std::pair<double,double> factor=getFactor(mode,As,Ad);
    double Fs=factor.first;
    double Fd=factor.second;
    //calculate the new RGB
    this->red=this->red*As*Fs+dst.getRed()*Ad*Fd;
    this->green=this->green*As*Fs+dst.getGreen()*Ad*Fd;
    this->blue=this->blue*As*Fs+dst.getBlue()*Ad*Fd;
    //calculate the new Alpha
    double newAlpha=As*Fs+Ad*Fd;
    //restore the RGB without the pre-multi of the Alpha
    this->red=this->red/newAlpha;
    this->green=this->green/newAlpha;
    this->blue=this->blue/newAlpha;
    //restore the new Alpah
    this->alpha=newAlpha*255.0;
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

std::pair<double,double> RS::BasicPoint::getFactor(blendMode mode,double As,double Ad){
    switch (mode)
    {
        case CLEAR:
            return {0,0};
            break;
        case SRC:
            return {1,0};
            break;
        case DST:
            return {0,1};
            break;
        case SRC_OVER:
            return {1,1-As};
            break;
        case DST_OVER:
            return {1-Ad,1};
            break;
        case SRC_IN:
            return {Ad,0};
            break;
        case DST_IN:
            return {0,As};
            break;
        case SRC_OUT:
            return {1-Ad,0};
            break;
        case DST_OUT:
            return {0,1-As};
            break;
        case SRC_ATOP:
            return {Ad,1-As};
            break;
        case DST_ATOP:
            return {1-Ad,As};
            break;
        case XOR:
            return {1-Ad,1-As};
            break;
        default:
            err("Unknow blend mode\n");
            return {-1,-1};
            break;
    }
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

bool RS::BasicLayer::haveSize() const{
    if(datamatrix.size()==0)return false;
    if(datamatrix[0].size()==0)return false;
    return true;
}

uint16_t RS::BasicLayer::getWidth() const{
    if(haveSize())return datamatrix.size();
    else return 0;
}

uint16_t RS::BasicLayer::getLength() const{
    if(haveSize())return datamatrix[0].size();
    else return 0;
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

void  RS::BasicLayer::getDataBuffer(dataBuffer& result) const{
    uint16_t width=0;
    uint16_t length=0;
    if(haveSize()){
        width=datamatrix.size();
        length=datamatrix[0].size();
    }
    result=dataBuffer(width,rowData(length));
    for(int i=0;i<width;++i){
        for(int j=0;j<length;++j){
            result[i][j]=datamatrix[i][j].getUint32();
        }
    }
}

bool RS::BasicLayer::squareRotate(){
    int n=datamatrix.size();
    for(int row=0;row<=(n-1)/2;++row)
    {
        for(int column=row;column<n-row-1;++column)
        {
            int a[2]={row,column};
            int b[2]={column,n-1-row};
            int c[2]={n-1-row,n-1-column};
            int d[2]={n-1-column,row};
            RS::BasicPoint tmp=datamatrix[d[0]][d[1]];
            datamatrix[d[0]][d[1]]=datamatrix[c[0]][c[1]];
            datamatrix[c[0]][c[1]]=datamatrix[b[0]][b[1]];
            datamatrix[b[0]][b[1]]=datamatrix[a[0]][a[1]];
            datamatrix[a[0]][a[1]]=tmp;
        }
    }
    return true; 
}

bool RS::BasicLayer::rectangleRotate(){
    if(!haveSize())return false;
    if(datamatrix.size()==datamatrix[0].size())return false;
    uint16_t length=datamatrix.size();
    uint16_t width=datamatrix[0].size();
    pointMatrix newDataMatrix(width,std::vector<RS::BasicPoint>(length));
    for(int i=0;i<length;++i){
        for(int j=0;j<width;++j){
            newDataMatrix[width-1-j][i]=datamatrix[i][j];
        }
    }
    datamatrix=newDataMatrix;
    return true;
}

bool RS::BasicLayer::rightRotate(){
    if(!haveSize())return false;
    if(datamatrix.size()==datamatrix[0].size()){
        if(squareRotate())return true;
    }else{
        if(rectangleRotate())return true;
    }
    return false;
}

bool RS::BasicLayer::leftRotate(){
    if(!haveSize())return false;
    if(datamatrix.size()==datamatrix[0].size()){
        for(int i=0;i<3;++i)if(!squareRotate())return false;
        return true;
    }else{
        for(int i=0;i<3;++i)if(!rectangleRotate())return false;
        return true;
    }
    return false;
}

bool RS::BasicLayer::upDownReverse(){
    if(!haveSize())return false;
    uint16_t low=0;
    uint16_t high=datamatrix.size()-1;
    while(low<=high){
        for(int i=0;i<datamatrix[0].size();++i){
            std::swap(datamatrix[low][i],datamatrix[high][i]);
        }
        ++low;
        --high;
    }
    return true;;
}

bool RS::BasicLayer::leftRightReverse(){
    if(!haveSize())return false;
    for(int i=0;i<datamatrix.size();++i){
        uint16_t left=0;
        uint16_t right=datamatrix[0].size()-1;
        while(left<=right){
            std::swap(datamatrix[i][left--],datamatrix[i][right--]);
        }
    }
    return false;
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

RS::BasicImage::BasicImage(){
    const std::string& name="default";
    this->name=name;
    uint16_t currentIndex=0;
    validLayer=0;
    totalLayer=0;
    current=0;
}

RS::BasicImage::BasicImage(const BasicLayer& aLayer,const std::string& name):name(name){
    uint16_t currentIndex=0;
    validLayer=0;
    totalLayer=0;
    current=0;
    insert(aLayer);
}


RS::BasicImage::BasicImage(const uint16_t width,const uint16_t length,const std::string& name){
    uint16_t currentIndex=0;
    validLayer=0;
    totalLayer=0;
    current=0;
    RS::BasicLayer aLayer(width,length);
    insert(aLayer);
}

RS::BasicImage::BasicImage(const dataBuffer& data){
    const std::string& name="default";
    uint16_t currentIndex=0;
    validLayer=0;
    totalLayer=0;
    current=0;
    RS::BasicLayer aLayer(data);
    insert(aLayer);
}


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

bool RS::BasicImage::reHash(){
    nameToIndex.clear();
    for(int i=0;i<layers.size();++i){
        if(nameToIndex.insert({layers[i].getLayerName(),i}).second==false)return false;
    }
    return true;
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
    if(layers[index].isValid())--validLayer;
    --totalLayer;
    layers.erase(layers.begin()+index);
    if(reHash())return true;
    err("NameToIndex reHash error.");
    return false;
}

bool RS::BasicImage::remove(const std::string& name){
    if(!nameToIndex.count(name)){
        err("No such layer\n");
        return false;
    }
    uint16_t index=nameToIndex[name];
    if(RS::BasicImage::remove(index))return true;
    return false;
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

bool RS::BasicImage::mergeLayer(const std::string& name1,const std::string& name2,blendMode mode){
    uint16_t index1=findByName(name1);
    uint16_t index2=findByName(name2);
    if(mergeLayer(index1,index2,mode))return true;
    return false;
}
bool RS::BasicImage::mergeLayer(const uint16_t index1,const uint16_t index2,blendMode mode){
    checkFit(index1,index2);
    if(mergeLayerCore(index1,index2,mode))return true;
    return false;
}

//--------------------------------------------------------------------
//-------------------------BasicImage_private-------------------------
//--------------------------------------------------------------------
bool RS::BasicImage::mergeLayerCore(const uint16_t index1,const uint16_t index2,blendMode mode){
    uint16_t width=layers[index1].getWidth();
    uint16_t length=layers[index1].getLength();
    //direct modigy the layerdata, no need to build a new matrix and reset the layer
    pointMatrix& dataSrc=layers[index1].getDataMatrix();
    pointMatrix& dataDst=layers[index2].getDataMatrix();
    for(int i=0;i<width;++i){
        for(int j=0;j<length;++j){
            dataSrc[i][j].blend(dataDst[i][j],mode);
        }
    }
    if(!remove(index2))return false;
    return true; 
}

//handle the different size merge
bool RS::BasicImage::mergeLayerCoreDiff(const uint16_t index1,const uint16_t index2,
                                        blendMode mode,uint16_t row,uint16_t column){
    uint16_t srcWidth=layers[index1].getWidth();
    uint16_t srcLength=layers[index1].getLength();
    uint16_t dstWidth=layers[index2].getWidth();
    uint16_t dstLength=layers[index2].getLength();
    uint16_t newWidth=std::max(srcWidth,(uint16_t)(row+dstWidth));
    uint16_t newLength=std::max(srcLength,(uint16_t)(column+dstLength));
    pointMatrix newMatrix(newWidth,rowPoint(newLength));
    for(int i=0;i<srcWidth;++i){
        for(int j=0;j<srcLength;++j){
            newMatrix[i][j].reInit()
        }
    }
    
}


bool RS::BasicImage::checkFit(const uint16_t index1,const uint16_t index2){
    if(!indexOK(index1)||!indexOK(index2))return false;
    RS::BasicLayer l1=layers[index1];
    RS::BasicLayer l2=layers[index2];
    if(!l1.haveSize()||!l2.haveSize())return false;
    if(l1.getWidth()!=l2.getWidth()||l1.getLength()!=l2.getLength())return false;
    return true;
}

bool RS::BasicImage::indexOK(const uint16_t index){
    return index>=0&&index<layers.size();
}

int16_t RS::BasicImage::findByName(const std::string& name){
    if(!nameToIndex.count(name)){
        err("No such layer name1\n");
        return -1;
    }
    return nameToIndex[name];
}






