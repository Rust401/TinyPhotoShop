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
    RS::BasicLayer l1(100,100);
    RS::BasicLayer l2;
    pointMatrix data(120,std::vector<RS::BasicPoint>(130));
    RS::BasicLayer l3(data,"bachi",120,true);
    l1.display();
    l2.display();
    l3.display();
    foo(l1);
    l1.display();
    pointMatrix diaosi=l1.getDataMatrix();
    std::cout<<diaosi.size()<<" "<<diaosi[0].size()<<std::endl;
    for(int i=0;i<5;++i){
        for(int j=0;j<5;++j)
        {
            printf("%hhu %hhu %hhu %hhu\n",diaosi[i][j].getRed(),diaosi[i][j].getAlpha()
            ,diaosi[i][j].getGreen(),diaosi[i][j].getBlue());
        }
    }

}