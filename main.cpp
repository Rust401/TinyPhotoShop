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


    dataBuffer haha(1024,std::vector<uint32_t>(1024,0xffffffff));
    dataBuffer baba={{0x11111111,0x22222222,0xffffffff},{0x33333333,0x44444444,0x3f3f3f3f},
        {0x11111111,0x22222222,0xffffffff},{0x33333333,0x44444444,0x3f3f3f3f}};
    dataBuffer caca(2048,std::vector<uint32_t>(2048,0x3f3f3f3f));
    /* RS::BasicImage dude(haha);
    dude.display();
    dude.insert("ruozhi",haha);
    dude.updateLayer(0,baba);
    dude.display();
    dude.displayHash(); */
    /* RS::BasicLayer keyi;
    keyi.display();
    keyi.displayData();
    keyi.setDataMatrix(haha);
    keyi.display();
    keyi.setDataMatrix(baba);
    keyi.display();
    keyi.displayData();  */
    

    /* uint32_t gege=0x3f3f3f3f;
    RS::BasicPoint caonima((uint32_t)gege);
    caonima.display();
    caonima.reInit((uint32_t)0xAAAAAAAA);
    caonima.display();
    std::cout<<std::endl;
    std::cout<<std::endl; */




    RS::BasicLayer l1(1024,1024);
    l1.setLayerName("doubi 1");
    RS::BasicLayer l2(2048,2048);
    l2.setLayerName("doubi 2");
    RS::BasicLayer l3(40,40);
    l3.setLayerName("doubi 3");
    RS::BasicLayer l4(81,81);
    l4.setLayerName("doubi 4");
    l4.setInvalid();
    RS::BasicImage img;
    img.insert(l1);
    img.insert(l2);
    img.insert(l3);
    img.insert(l4);
    img.updateLayer("doubi 4",caca);
    img.duplicate("doubi 4");
    img.duplicate("doubi 3");
    img.swap("doubi 1","doubi 2");
    img.displayHash();
    
    img.display();
    


    
    

/*     img.displayHash(); */
    /* img.swap("doubi 3","doubi 4");
    img.displayHash();
    img.swap("doubi 2","doubi 3");
    img.displayHash(); */
/*     img.display(); */
    

}