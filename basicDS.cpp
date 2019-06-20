#include "basicDS.h"
using namespace RS;
void BasicPoint::reInit(){
    red=0;
    green=0;
    blue=0;
    alpha=0;
}

void BasicPoint::display(){
    printf("Point:%p Red:%d Green: %d Blue: %d Alpha: %d\n",
        this,(uint16_t)red,(uint16_t)green,(uint16_t)blue,(uint16_t)alpha);
}


BasicPoint& BasicPoint::operator=(const BasicPoint& p)
{
    red=p.red;
    green=p.green;
    blue=p.blue;
    alpha=p.alpha;
    return *this;
}