//#define TEST1
#define TEST2
//#define TEST3
#include "basicDS.h"


void foo(RS::BasicLayer& srcs){
    pointMatrix data(110,std::vector<RS::BasicPoint>(1200));
    srcs.setDataMatrix(data);
}

#ifdef TEST1
int main()
{
    //The test matir
    dataBuffer haha(1024,rowData(1024,0xffffffff));
    dataBuffer baba={{0x1f2f3f4f,0x22222222,0xffffffff},
                     {0x33333333,0x44444444,0x3f3f3f3f},
                     {0x11111111,0x22222222,0xffffffff},
                     {0x33333333,0x44444444,0x3f3f3f3f}};
    dataBuffer caca(2048,rowData(2048,0x3f3f3f3f));

    //The array use to pass the start and end index of the taylor function
    //rowStart columnStart rowEnd columnEnd
    std::vector<uint16_t> array={30,30,60,60};

    //Here we inital the layer use the different method
    //--------------------------------------------------

    //initial a layer with size 1024*1024, all Point are {0,0,0,0}
    //Don't forget to give the layer a name for the reason that layer
    //will be insert into a hashmap with the key of the name to get the
    //index in the layer vector
    //the default name of the initalized layer is "default"
    RS::BasicLayer l1(1024,1024);
    l1.setLayerName("doubi 1");

    //intial a layer with size 2048*2048
    RS::BasicLayer l2(2048,2048);
    l2.setLayerName("doubi 2");


    //40*40 layer
    RS::BasicLayer l3(40,80);
    l3.setLayerName("doubi 3");

    //use the dataBuffer to initial the layer
    //baba 3*3
    RS::BasicLayer l4(baba);
    l4.setLayerName("doubi 4");

    //display the data layout of the layer
    l4.displayData();

    std::cout<<std::endl;

    //up down reverse the layer 
    l4.upDownReverse();
    l4.displayData();

    //left right reverse the layer
    l4.leftRightReverse();
    std::cout<<std::endl;
    l4.displayData();

    //right rotate the layer
    l4.rightRotate();
    std::cout<<std::endl;
    l4.displayData();

    std::cout<<std::endl;

    //left rotate the layer
    l4.leftRotate();
    l4.displayData();

    //set the state of the l4 invalid
    l4.setInvalid();

    //initial the image object with no layer
    RS::BasicImage img;

    //insert the 4 layer
    img.insert(l1);
    img.insert(l2);
    img.insert(l3);
    img.insert(l4);

    //show the key-value of the hash  name - index
    img.displayHash();
    img.reHash();
    img.displayHash();

    std::cout<<std::endl;

    //cur the layer named "doubi 1" with the data in the array
    img.taylor("doubi 1",array);

    //update the layer named "doubi4" with the databuffer
    img.updateLayer("doubi 4",caca);

    //duplicate the layer named "doubi 4" and the new layer named "doubi 4_dup"
    img.duplicate("doubi 4");
    //duplicate the layer named "doubi 3" and the new layer named "doubi 3_dup"
    img.duplicate("doubi 3");

    //swap the position of the layer named "doubi 1" and "doubi 2"
    img.swap("doubi 1","doubi 2");

    //show the key-value of the hash  name - index
    img.displayHash();
    img.reHash();
    img.displayHash();

    //get the layer and put the layer to the buffer gived
    RS::BasicLayer caiji=img.getLayer("doubi 1");
    dataBuffer testbuffer;
    caiji.getDataBuffer(testbuffer);

    //remove the layer
    img.remove("doubi 1");
    img.remove("doubi 2");
    img.remove("doubi 3");
    img.remove("doubi 4");

    img.displayHash();

    
    //show the whole info of the image
    img.display();     
}
#endif

#ifdef TEST2
int main()
{
    dataBuffer haha(5,rowData(5,0x64646464));
    dataBuffer gaga(5,rowData(5,0x28282828));
    dataBuffer baba={{0x1f2f3f4f,0x22222222,0xffffffff},
                     {0x33333333,0x44444444,0x3f3f3f3f},
                     {0x11111111,0x22222222,0xffffffff},
                     {0x33333333,0x44444444,0x3f3f3f3f}};
    dataBuffer caca(2048,rowData(2048,0x3f3f3f3f));

    RS::BasicLayer l1(haha);
    l1.setLayerName("dude1");
    RS::BasicLayer l2(gaga);
    l2.setLayerName("dude2");
    RS::BasicLayer l3(caca);
    l3.setLayerName("dude3");
    RS::BasicLayer l4(baba);
    l4.setLayerName("dude4");

    RS::BasicImage img;
    img.insert(l1);
    img.insert(l2);
    img.insert(l3);
    img.insert(l4);
    

    img.mergeLayer("dude1","dude2",SRC);
    RS::BasicLayer pick=img.getLayer(0);
    pick.displayData();

    img.display();
    img.displayHash();
}
#endif

#ifdef TEST3
int main()
{
    for(int i=0;i<=XOR;++i){
        RS::BasicPoint p1(100,100,100,100);
        RS::BasicPoint p2(50,50,50,100);
        p1.blend(p2,(blendMode)i);
        p1.display();
    }
}
#endif
