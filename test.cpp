#include "image.hpp"

using namespace dude;
 
int main()
{
    Point a,b,c;
    std::cout<<sizeof(a)<<std::endl;
    a.init();
    std::cout<<a.getRed()<<" "<<a.getBlue()<<" "<<a.getGreen()<<std::endl;
    std::cout<<a.getAlpha()<<" "<<a.getLayerNumbe()<<std::endl;
    return 0;
}