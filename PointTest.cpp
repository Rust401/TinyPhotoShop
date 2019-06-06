#include "image.hpp"

using namespace dude;
//This is the test for point
int main()
{
    dude::Point p1;
    p1.init();
    dude::Point px(20,20,20,20,0);
    //std::cout<<(int)p1.getLayerNumbe()<<std::endl;
    dude::Point p2(10,10,10,20,0);
    //p1=(p1+p2);
    std::vector<int> dudes={p1.getRed(),p1.getBlue(),p1.getGreen(),p1.getAlpha(),p1.getLayerNumbe()};
    for(auto dude:dudes)std::cout<<dude<<std::endl;
    return 0;
}