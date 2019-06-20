#include "basicDS.h"

int main()
{
    RS::BasicPoint p1;
    RS::BasicPoint p2(1,2,3);
    RS::BasicPoint p3(4,5,6,7);

    p1.display();
    p2.display();
    p3.display();

    p1=p2;
    p1.display();


    std::vector<std::vector<RS::BasicPoint>> a(10,std::vector<RS::BasicPoint>(10,RS::BasicPoint()));

}