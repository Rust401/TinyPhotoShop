#include "basicDS.h"
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