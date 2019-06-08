#ifndef IMAGE_HPP
#define IMAGE_HPP


#include<bits/stdc++.h>

#define pointMatrix std::vector<std::vector<Point> >

namespace dude
{
class Point
{
private:
    uint8_t red,blue,green,alpha;
    uint8_t layerNumber;
public:
    Point(){}
    Point(uint8_t red,uint8_t blue,uint8_t green,uint8_t alpha=0,uint8_t layerNumber=0):
        red(red),blue(blue),green(green),alpha(alpha),layerNumber(layerNumber){}
    void init(){memset(this,0,5);}
    uint8_t getRed() const{return this->red;}
    uint8_t getBlue() const{return this->blue;}
    uint8_t getGreen() const{return this->green;}
    uint8_t getAlpha() const{return this->alpha;}
    uint8_t getLayerNumbe() const{return this->layerNumber;}


    void setRed(uint8_t value){this->red=value;}
    void setBlue(uint8_t value){this->blue=value;}
    void setGreen(uint8_t value){this->green=value;}
    void setAlpha(uint8_t value){this->alpha=value;}
    void setLayerNumber(uint8_t layerNumber){this->layerNumber=layerNumber;}

    Point& operator+(const Point& otherPoint){
        uint8_t alpha1=alpha;
        uint8_t alpha2=otherPoint.getAlpha();       
        //map the alpha from 0-255 to 0.0-1.0
        double alphaFront=alpha/256.0;
        //get update the new color and the alpha
        red=(int8_t)(red*(1-alphaFront)+otherPoint.getRed()*alphaFront);
        blue=(int8_t)(blue*(1-alphaFront)+otherPoint.getBlue()*alphaFront);
        green=(int8_t)(green*(1-alphaFront)+otherPoint.getGreen()*alphaFront);
        alpha=(int8_t)(alpha+otherPoint.getAlpha()*(1-alphaFront));
        //return the first point
        return *this;
    }
};

//alloc the object on the heap, don't in the stack!!!
class Layer
{
private:
    uint8_t layerNumber;
    uint16_t length;
    uint16_t width;
    bool isValid;
    std::string name;
public:
    std::vector<std::vector<Point> >* matrix;
    Layer(){}
    Layer(uint8_t layerNumber,uint16_t length,uint16_t width,std::string name,std::vector<std::vector<Point> >* matrix,bool isValid=true):
        layerNumber(layerNumber),length(length),width(width),name(name),matrix(matrix){}

    ~Layer(){
        if(matrix!=nullptr)delete matrix;
    }
    void init(){
        layerNumber=0;
        length=1024;
        width=1024;
        isValid=true;
        name="";
        matrix=new pointMatrix(1024,std::vector<Point>(1024,Point(0,0,0,0,0)));
    }

    void reInit(uint8_t layerNumber,uint16_t length,uint16_t width,bool isValid,std::string name)
    {
        this->layerNumber=layerNumber;
        this->length=length;
        this->width=width;
        this->isValid=isValid;
        this->name=name;
        delete matrix;
        matrix=new pointMatrix(width,std::vector<Point>(length,Point(0,0,0,0,0)));
    }
    
    void displayLayerInfo(){
        std::cout<<"//--------------------------------------------------------------------//"<<std::endl;
        std::cout<<"LayerNubmer  :"<<(int16_t)layerNumber<<std::endl;
        std::cout<<"Length       :"<<length<<std::endl;
        std::cout<<"Width        :"<<width<<std::endl;
        std::cout<<"IsValid      :"<<isValid<<std::endl;
        std::cout<<"MatrixAddr   :"<<matrix<<std::endl;
        std::cout<<"//--------------------------------------------------------------------//"<<std::endl;
    }

    void setLayerNumber(uint8_t val){layerNumber=val;}

    void setLenth(uint16_t newLength){
        this->length=newLength;
        //use a new matrix to handle the data
        std::vector<std::vector<Point> > newMatrix(width,std::vector<Point>(length));
        //change the matrix     TODO!!!

        //write the new matrix back to the original matrix     TODO!!!

    }
    void setWith(uint16_t newWidth){
        this->width=newWidth;
        //use a new matrix to handle the data
        std::vector<std::vector<Point> > newMatrix(width,std::vector<Point>(length));
        //change the matrix     TODO!!!

        //write the new matrix back to the original matrix     TODO!!!

    }
    uint16_t getLength(){return this->length;}
    uint16_t getWidth(){return this->width;}
    std::vector<std::vector<Point> >* getMatrix(){return matrix;}
    void setName(std::string name){this->name=name;}
    void setValid(){this->isValid=true;}
    void setInvalid(){this->isValid=false;}
};



class Image
{
private:
    std::vector<Layer*> layers;
    std::string name;
    Layer* toDisplay;
    Layer* tmpLayer;
public:
    Image(){toDisplay=nullptr;tmpLayer=nullptr;}
    Image(std::vector<Layer*>& layers,std::string& name,Layer* toDisplay,Layer* tmpLayer):
        layers(layers),name(name),toDisplay(toDisplay),tmpLayer(tmpLayer){}

    ~Image(){
        for(auto dude: layers){
            if(dude!=nullptr){
                delete dude;
                dude=nullptr;
            }
        }
    }

    void Init(){
        if(layers.size()!=0)layers.clear();
        Layer* p=new Layer();
        p->init();
        layers.push_back(p);
        name="";
        toDisplay=layers[0];
        tmpLayer=layers[0];
    }
    

    Layer* getLayerToDisplayPointer(){return toDisplay;}
    Layer* getLayerCurrentPointer(){return tmpLayer;}
    void pushLayerFront(Layer* layer){
        if(layer!=nullptr)layers.push_back(layer);
        if(layers.size()>1)std::swap(layers[1],layers.back());
    }
    void pushLayerBack(Layer* layer){
        if(layer!=nullptr)layers.push_back(layer);
    }
    
    Layer* LayerMerge(uint8_t index1,uint8_t index2){
        Layer* layer1=layers[index1];
        Layer* layer2=layers[index2];
        
        if(layer1->getLength()!=layer2->getLength())return nullptr;
        if(layer1->getWidth()!=layer2->getLength())return nullptr;

        std::vector<std::vector<Point> > matrix1=*(layer1->getMatrix());
        std::vector<std::vector<Point> > matrix2=*(layer2->getMatrix());
        
        //To be modified because the merge of two layer actully depend on the alpha value
        for(int i=0;i<layer1->getWidth();++i){
            for(int j=0;j<layer1->getLength();++j){
                (*tmpLayer->matrix)[i][j].setRed((matrix1[i][j].getRed()+matrix2[i][j].getRed())/2);
                (*tmpLayer->matrix)[i][j].setBlue((matrix1[i][j].getBlue()+matrix2[i][j].getBlue())/2);
                (*tmpLayer->matrix)[i][j].setGreen((matrix1[i][j].getGreen()+matrix2[i][j].getGreen())/2);
                (*tmpLayer->matrix)[i][j].setAlpha((matrix1[i][j].getAlpha()+matrix2[i][j].getAlpha())/2);
            }
        }
        //delete the layer from the layers
        layers.erase(layers.begin()+index1);
        layers.erase(layers.begin()+index2);

        //notice we should set the layer merged the layerNumber, and then return it
        //should we put it right into the vector? in the front or int the end?
        return tmpLayer;
    }

    Layer* getTheLayerToDisplay()
    {
        int8_t flag=1;
        //just return the top layer
        if(1==flag){
            if(layers.size()==0)return nullptr;
            toDisplay=layers.back();
        }
        //merge all the valid layer with the layer number and their alpah
        if(0==flag){
            if(layers.size()==0)return nullptr;
            int16_t length=layers[0]->getLength();
            int16_t width=layers[0]->getWidth();
            for(int i=0;i<width;++i){
                for(int j=0;j<length;++j){
                    for(int k=0;k<layers.size();++k){
                        //just a demo, need to modifie
                        (*toDisplay->matrix)[i][j].setRed((*toDisplay->matrix)[i][j].getRed()+(*layers[k]->matrix)[i][j].getRed());
                        (*toDisplay->matrix)[i][j].setBlue((*toDisplay->matrix)[i][j].getBlue()+(*layers[k]->matrix)[i][j].getBlue());
                        (*toDisplay->matrix)[i][j].setGreen((*toDisplay->matrix)[i][j].getGreen()+(*layers[k]->matrix)[i][j].getGreen());
                        (*toDisplay->matrix)[i][j].setAlpha((*toDisplay->matrix)[i][j].getAlpha()+(*layers[k]->matrix)[i][j].getAlpha());
                    }
                }
            }
        }
        return toDisplay;
    }
};
}



#endif //POINT_H