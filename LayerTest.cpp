#include "image.hpp"

using namespace dude;

struct shabi
{
    bool shabime;
};

int main()
{
    Layer l1,l2;
    std::cout<<sizeof(l1)<<std::endl;
    std::string* p=new std::string;
    std::string dude="nishishabi";
    bool tf=true;
    std::vector<int> v1;
    std::vector<double> v2;
    std::vector<Layer> v3;
    shabi baichi;
    
    

    std::cout<<sizeof(p)<<"  "<<sizeof(dude)<<" "<<sizeof(tf)<<std::endl;

    std::cout<<sizeof(v1)<<" "<<sizeof(v2)<<" "<<sizeof(v3)<<std::endl;
    return 0;

}