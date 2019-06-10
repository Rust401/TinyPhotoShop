#include "image.hpp"
using namespace dude;

int main()
{
    Image i1;
    i1.Init();

    Layer* l1=new Layer();
    l1->reInit(1,2000,2000,true,"layer1");

    Layer* l2=new Layer();
    l2->reInit(2,2000,2000,true,"layer2");


    Layer* l3=new Layer();
    l3->reInit(3,2000,2000,true,"layer3");

    Layer* l4=new Layer();
    l4->reInit(4,2000,2000,true,"layer4");

    Layer* l5=new Layer();
    l5->reInit(5,2000,2000,true,"layer5");

    Layer* l6=new Layer();
    l6->reInit(6,2000,2000,true,"layer6");

    //0
    i1.pushLayerBack(l1);//1
    i1.pushLayerBack(l2);//2
    i1.pushLayerBack(l3);//3
    i1.pushLayerBack(l4);//4
    i1.pushLayerBack(l5);//5
    i1.pushLayerBack(l6);//6
                     
    /*
    for(auto dude:*(i1.getLayersPointer())){
        
        if(dude==nullptr)std::cout<<"nullptr"<<std::endl;
        else std::cout<<dude<<std::endl;
    }
    */
    i1.layerMerge(1,2);
    i1.layerMerge(3,5);


    i1.displayLayerInfo();
    return 0;

}