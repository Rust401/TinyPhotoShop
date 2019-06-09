#include "image.hpp"
using namespace dude;


//The test of the class
int main()
{
    Image i1;
    i1.Init();


    Layer* l1=new Layer();

    l1->init();
    l1->reInit(1,100,100,true,"shabi");

    i1.pushLayerBack(l1);
    Layer* l2=new Layer();
    l2->init();
    l2->reInit(3,500,600,true,"baichi");

    Layer* l3=new Layer();
    l3->init();
    l3->reInit(5,1500,1500,true,"ruozhi");

    i1.pushLayerBack(l2);
    i1.pushLayerBack(l3);

    layerVector* p=i1.getLayersPointer();
    for(int i=0;i<(*p).size();++i){
        (*p)[i]->displayLayerInfo();
    }

    //i1.displayLayerInfo();
    return 0;
}