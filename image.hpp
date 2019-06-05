#ifndef IMAGE_HPP
#define IMAGE_HPP

#include<bits/stdc++.h>

namespace dude
{
class Point
{
private:
    int8_t red,blue,green,alpha;
    int8_t layerNumber;
public:
    Point();
    Point(int8_t red=0,int8_t blue=0,int8_t green=0,int8_t alpha=0,int8_t layerNumber=0):
        red(red),blue(blue),green(green),alpha(alpha){}
    

    int8_t getRed() const{return this->red;}
    int8_t getBlue() const{return this->blue;}
    int8_t getGreen() const{return this->green;}
    int8_t getAlpha() const{return this->alpha;}
    int8_t getLayerNumbe() const{return this->alpha;}


    void setRed(int8_t value){this->red=value;}
    void setBlue(int8_t value){this->blue=value;}
    void setGreen(int8_t value){this->green=value;}
    void setAlpha(int8_t value){this->alpha=value;}
    void setLayerNumber(int8_t layerNumber){this->layerNumber=layerNumber;}
};

class Layer
{
private:
    uint8_t layerNumber;
    uint16_t length;
    uint16_t width;
    bool isValid;
    std::string name;
public:
    std::vector<std::vector<Point>> matrix;
    Layer();
    Layer(uint8_t layerNumber,uint16_t length,uint16_t width,std::string name,std::vector<std::vector<Point>>& matrix,bool isValid=true):
        layerNumber(layerNumber),length(length),width(width),name(name),matrix(matrix){}
    
    void setLayerNumber(uint8_t val){layerNumber=val;}

    void setLenth(uint16_t newLength){
        this->length=newLength;
        //use a new matrix to handle the data
        std::vector<std::vector<Point>> newMatrix(width,std::vector<Point>(length));
        //change the matrix     TODO!!!

        //write the new matrix back to the original matrix     TODO!!!

    }
    void setWith(uint16_t newWidth){
        this->width=newWidth;
        //use a new matrix to handle the data
        std::vector<std::vector<Point>> newMatrix(width,std::vector<Point>(length));
        //change the matrix     TODO!!!

        //write the new matrix back to the original matrix     TODO!!!

    }
    uint16_t getLength(){return this->length;}
    uint16_t getWidth(){return this->width;}
    std::vector<std::vector<Point>>& getMatrix(){return matrix;}
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
    Image();
    Image(std::vector<Layer*>& layers,std::string& name,Layer* toDisplay,Layer* tmpLayer):
        layers(layers),name(name),toDisplay(toDisplay),tmpLayer(tmpLayer){}
    
    Layer* LayerMerge(uint8_t index1,uint8_t index2){
        Layer* layer1=layers[index1];
        Layer* layer2=layers[index2];
        
        if(layer1->getLength()!=layer2->getLength())return nullptr;
        if(layer1->getWidth()!=layer2->getLength())return nullptr;

        std::vector<std::vector<Point>>& matrix1=layer1->getMatrix();
        std::vector<std::vector<Point>>& matrix2=layer2->getMatrix();
        
        //To be modified because the merge of two layer actully depend on the alpha value
        for(int i=0;i<layer1->getWidth();++i){
            for(int j=0;j<layer1->getLength();++j){
                tmpLayer->matrix[i][j].setRed((matrix1[i][j].getRed()+matrix2[i][j].getRed())/2);
                tmpLayer->matrix[i][j].setBlue((matrix1[i][j].getBlue()+matrix2[i][j].getBlue())/2);
                tmpLayer->matrix[i][j].setGreen((matrix1[i][j].getGreen()+matrix2[i][j].getGreen())/2);
                tmpLayer->matrix[i][j].setAlpha((matrix1[i][j].getAlpha()+matrix2[i][j].getAlpha())/2);
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
                        toDisplay->matrix[i][j].setRed(toDisplay->matrix[i][j].getRed()+layers[k]->matrix[i][j].getRed());
                        toDisplay->matrix[i][j].setBlue(toDisplay->matrix[i][j].getBlue()+layers[k]->matrix[i][j].getBlue());
                        toDisplay->matrix[i][j].setGreen(toDisplay->matrix[i][j].getGreen()+layers[k]->matrix[i][j].getGreen());
                        toDisplay->matrix[i][j].setAlpha(toDisplay->matrix[i][j].getAlpha()+layers[k]->matrix[i][j].getAlpha());
                    }
                }
            }
        }
        return toDisplay;
    }
};
}


#endif //POINT_H