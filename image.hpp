#ifndef IMAGE_HPP
#define IMAGE_HPP


#include<bits/stdc++.h>

#define pointMatrix std::vector<std::vector<Point> >
#define layerVector std::vector<dude::Layer*>

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

    //The Layer::init() give the layer a 1024*1024 Point matrix with all the Point obj set to the zero
    //Every time after new a Layer obj in the heap, be sure to init() or reInit()
    void init(){
        layerNumber=0;
        length=1024;
        width=1024;
        isValid=true;
        name="";
        matrix=new pointMatrix(1024,std::vector<Point>(1024,Point(0,0,0,0,0)));
    }

    //Delete the current layer matrix in the heap and give it the new size you want
    //All the Points in the layer are blank
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
        std::cout<<std::endl;
        std::cout<<"LayerNubmer  :"<<(int16_t)layerNumber<<std::endl;
        std::cout<<"Length       :"<<length<<std::endl;
        std::cout<<"Width        :"<<width<<std::endl;
        std::cout<<"IsValid      :"<<isValid<<std::endl;
        std::cout<<"MatrixAddr   :"<<matrix<<std::endl;
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
    uint16_t getLayerNumber(){return this->layerNumber;}
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
    Layer* currentLayer;
public:
    //default constructor do nothing and set all the pointer to nullptr
    Image(){toDisplay=nullptr;tmpLayer=nullptr;currentLayer=nullptr;}

    Image(std::vector<Layer*>& layers,std::string& name,Layer* toDisplay,Layer* tmpLayer):
        layers(layers),name(name),toDisplay(toDisplay),tmpLayer(tmpLayer){}

    //We allocate all the Layer obeject in the heap use the "new Layer" method
    //So the d-constructor shold release the memory in the heap address
    //---------------------------------------------------
    ~Image(){
        //Here may contain a server bug when delelete  the layer on the heap
        
        std::unordered_set<Layer*> aSet;
        for(int i=0;i<layers.size();++i){
            std::cout<<"fuck"<<std::endl;
            if(layers[i]==nullptr)continue;
            if(!aSet.count(layers[i])){
                aSet.insert(layers[i]);
                std::cout<<"before delte"<<std::endl;
                if(nullptr!=layers[i])if(i==1)delete layers[i];
                std::cout<<"after delete"<< std::endl;
            }     
        }   
    }

    //The init() function will give the image vector a blank layer will size of 1024*1024
    //with all the data in the point to be zero
    void Init(){
        if(layers.size()!=0)layers.clear();
        Layer* p=new Layer();
        p->init();
        layers.push_back(p);
        name="";
        toDisplay=layers[0];
        tmpLayer=layers[0];
        currentLayer=layers[0];
    }

    //===----------------------------------------------------------------------===//
    // Get funciotion
    //===----------------------------------------------------------------------===//

    //Return the number of the layer in the current image
    int16_t getLayerSize() const {return layers.size();}
    
    //Get the reference of the layer vector 
    std::vector<Layer*>& getLayers() {return layers;}

    //Get the pointer of the layer vector
    std::vector<Layer* >* getLayersPointer() {return &layers;}

    //Get the pointer to the layer to be display
    Layer* getLayerToDisplay() const {return toDisplay;}

    //Get the pointer to the layer which is selected
    Layer* getCurrentLayer() const{return currentLayer;}


    //===----------------------------------------------------------------------===//
    // Set function
    //===----------------------------------------------------------------------===//

    //implicit set the display layer to layer with some index
    void setLayerToDisplay(int16_t index){
        if(index>layers.size())return;
        if(layers[index]!=nullptr)toDisplay=layers[index];
    }

    //implicit set the current layer to layer with some index
    void setCurrentLayer(int16_t index){
        if(index>layers.size())return;
        if(layers[index]!=nullptr)currentLayer=layers[index];
    }

    //Display the layer info to the stdout
    void displayLayerInfo(){
        std::cout<<"//----------------Layer Info-----------------//"<<std::endl;
        std::cout<<"name:         "<<name<<std::endl;
        std::cout<<"layerSize:    "<<getLayerSize()<<std::endl;
        std::cout<<"toDisplay:    "<<(toDisplay==nullptr?0:toDisplay)<<std::endl;
        std::cout<<"tmpLayer:     "<<(tmpLayer==nullptr?0:tmpLayer)<<std::endl;
        std::cout<<"currentLayer  "<<(currentLayer==nullptr?0:currentLayer)<<std::endl;
        for(auto dude:layers){
            if(dude!=nullptr)dude->displayLayerInfo();
            else std::cout<<"\nZombie Layer \n";
        }
        return;
    }

    //===----------------------------------------------------------------------===//
    // Transfer function
    //===----------------------------------------------------------------------===//

    //Push a new layer to front of the vector
    void pushLayerFront(Layer* layer){
        if(layer!=nullptr)layers.insert(layers.begin(),layer);
    }

    //push a new layer to the end of the vector
    void pushLayerBack(Layer* layer){
        if(layer!=nullptr)layers.push_back(layer);
    }

    //Check before the sergery
    bool canBeMerged(uint8_t index1,uint8_t index2){
        //The check before the sergery
        if(index1==index2){
            std::cout<<"The same layer, do nothin"<<std::endl;
            return false;
        }
        if(index1>=layers.size()||index2>=layers.size()){
            std::cout<<"No such many layers dude"<<std::endl;
            return false;
        }
        if(layers[index1]==nullptr||layers[index2]==nullptr){
            std::cout<<"One of the layers is empty, nothing to do"<<std::endl;
            return false;
        }
        if(layers[index1]->getLength()!=layers[index2]->getLength()||layers[index1]->getWidth()!=layers[index2]->getWidth()){
            std::cout<<"The size of two layer is not the same"<<std::endl;
            return false;
        }
        return true;
    }

    //The core function to merge the l1,l2 and put the result into l3
    //This is not the point to be fucked
    bool mergeCore(Layer* l1,Layer* l2,Layer* l3){
        for(int i=0;i<l1->getWidth();++i){
            for(int j=0;j<l1->getLength();++j){
                (*(l3->matrix))[i][j].setRed(((*(l1->matrix))[i][j].getRed()+(*(l1->matrix))[i][j].getRed())/2);
                (*(l3->matrix))[i][j].setBlue(((*(l1->matrix))[i][j].getBlue()+(*(l1->matrix))[i][j].getBlue())/2);
                (*(l3->matrix))[i][j].setGreen(((*(l1->matrix))[i][j].getGreen()+(*(l1->matrix))[i][j].getGreen())/2);
                (*(l3->matrix))[i][j].setAlpha(((*(l1->matrix))[i][j].getAlpha()+(*(l1->matrix))[i][j].getAlpha())/2);
            }
        }
        if(nullptr!=l3)return true;
        return false;
       return true;
    }
    //give the index of the layer and merge it, then replace the first layer and delete the second layer
    //The algorithm here is just get the mean of the two attributes of 2 pont mapped
    //The core function, change here
    //BUG HERE TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
    bool layerMerge(uint8_t index1,uint8_t index2){

        //The check before the sergery
        if(!canBeMerged(index1,index2))return false;
        //Check ended

        Layer* l1=layers[index1];
        Layer* l2=layers[index2];

        //The ner layer to save the result
        Layer* l3=new Layer();
        l3->reInit(l2->getLayerNumber(),l1->getLayerNumber(),l1->getWidth(),true,"");

        //The algorithm here is just get the mean of the two attributes of 2 pont mapped
        //The core function, change here
        if(!mergeCore(l1,l2,l3))return false;
        
        delete l1;
        layers[index1]=nullptr;
        delete l2;
        layers[index2]=nullptr;
        layers[index1]=l3;
        if(l3!=nullptr)return true;
        std::cout<<"lose the layer merged, merge failed"<<std::endl;
        return false;
    }

    //This method merge two layer and push them to the end of the vector without delete the former layer
    bool layerMergeAndPushBack(uint8_t index1,uint8_t index2){

        //The check before the sergery
        if(!canBeMerged(index1,index2))return false;
        //Check ended

        Layer* l1=layers[index1];
        Layer* l2=layers[index2];

        //The ner layer to save the result
        Layer* l3=new Layer();
        l3->reInit(l2->getLayerNumber(),l1->getLayerNumber(),l1->getWidth(),true,"");

        //The algorithm here is just get the mean of the two attributes of 2 pont mapped
        //The core function, change here
        if(!mergeCore(l1,l2,l3))return false;
        
        if(nullptr!=l3){
            layers.push_back(l3);
            return true;
        }
        std::cout<<"lose the layer merged, merge failed"<<std::endl;
        return false;
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