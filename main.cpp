#include "basicDS.h"

void foo(RS::BasicLayer& srcs){
    pointMatrix data(110,std::vector<RS::BasicPoint>(1200));
    srcs.setDataMatrix(data);
}


int main()
{

    /* std::vector<std::vector<RS::BasicPoint>> a(1,std::vector<RS::BasicPoint>(1,RS::BasicPoint(0,0,0,0)));
    std::vector<std::vector<RS::BasicPoint>> b(1,std::vector<RS::BasicPoint>(1));
    std::cout<<"shifoer"<<std::endl;
    pointMatrix shabi=a; */

    /* int a=100;
    const int& b=a;
    std::cout<<&b<<std::endl; */


    dataBuffer haha={{0x11111111,0x22222222,0xffffffff},{0x33333333,0x44444444,0x3f3f3f3f}};
    RS::BasicLayer keyi(haha);
    keyi.display();
    keyi.displayData();
    

    /* uint32_t gege=0x3f3f3f3f;
    RS::BasicPoint caonima((uint32_t)gege);
    caonima.display();
    caonima.reInit((uint32_t)0xAAAAAAAA);
    caonima.display();
    std::cout<<std::endl;
    std::cout<<std::endl; */




/*     RS::BasicLayer l1(100,100);
    l1.setLayerName("doubi 1");
    RS::BasicLayer l2(1200,10);
    l2.setLayerName("doubi 2");
    RS::BasicLayer l3(5,5);
    l3.setLayerName("doubi 3");
    RS::BasicLayer l4(7,120);
    l4.setLayerName("doubi 4");
    l4.setInvalid();
    RS::BasicImage img;
    img.insert(l1);
    img.insert(l2);
    img.insert(l3);
    img.insert(l4); */


    
    

/*     img.displayHash(); */
    /* img.swap("doubi 3","doubi 4");
    img.displayHash();
    img.swap("doubi 2","doubi 3");
    img.displayHash(); */
/*     img.display(); */
    

}