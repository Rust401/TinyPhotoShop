#include "image.hpp"

using namespace dude;


int main()
{
    Layer l1;
    l1.init();
    std::cout<<l1.matrix.size()<<std::endl;
    std::cout<<l1.matrix[0].size()<<std::endl;
    Point p=l1.matrix[0][0];
    printf("%d %d %d\n",p.getRed(),p.getAlpha(),p.getLayerNumbe());
    /*
    for(int i=0;i<1024;++i)
        for(int j=0;j<1024;++j)
            printf("%d %d %d\n",p.getRed(),p.getAlpha(),p.getLayerNumbe());
            */
    l1.displayLayerInfo();
    return 0;
}