#include<bits/stdc++.h>

class A
{
private:
    int d;
public:
    int a,b;
    int* getaddr1(){
        return &d;
    }
private:
    int h;
public:
    int* getaddr2(){
        return &h;
    }
};
class B:public A{
public:
    int c;
};

int main(){
    B dude;
    std::cout<<sizeof(dude)<<std::endl;
    std::cout<<"a"<<&dude.a<<std::endl;
    std::cout<<"b"<<&dude.b<<std::endl;
    std::cout<<"c"<<&dude.c<<std::endl;
    std::cout<<"d"<<dude.getaddr1()<<std::endl;
    std::cout<<"h"<<dude.getaddr2()<<std::endl;
    return 0;

}