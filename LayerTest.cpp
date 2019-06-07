#include "image.hpp"

using namespace dude;


int main()
{
    Layer l1;
    //l1.displayLayerInfo();
    l1.init();
    //l1.displayLayerInfo();
    Point p=(*l1.matrix)[0][0];
    printf("%d %d %d\n",p.getRed(),p.getAlpha(),p.getLayerNumbe());
    l1.reInit(2,500,500,true,"baichi");
    //l1.displayLayerInfo();

    int* dude;
    if(dude)std::cout<<dude<<std::endl;
    dude=new int[10];
    if(dude)std::cout<<dude<<std::endl;



    l1.reInit(2,500,500,true,"baichi");
    //l1.displayLayerInfo();
    return 0;
}