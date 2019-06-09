#include "image.hpp"
using namespace dude;

int main()
{
    Image i1;
    i1.Init();
    //i1.displayLayerInfo();

    Layer* l1=new Layer();
    //l1->displayLayerInfo();
    l1->reInit(1,2000,2000,true,"layer1");
    //l1->displayLayerInfo();

    Layer* l2=new Layer();
    l2->reInit(2,2000,2000,true,"layer2");


    Layer* l3=new Layer();
    l3->reInit(3,2000,2000,true,"layer3");

    Layer* l4=new Layer();
    l4->reInit(3,2000,2000,true,"layer4");


    //0
    i1.pushLayerBack(l1);//1
    i1.pushLayerBack(l2);//2
    i1.pushLayerBack(l3);//3
    i1.pushLayerBack(l4);//4
                            


    //A server bug to fix
    i1.layerMerge(3,4);

    
    
    i1.displayLayerInfo();
}