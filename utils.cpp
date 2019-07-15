#include "utils.h"

void err(const std::string& str){
    perror(str.c_str());
}

std::string getPostfix(const std::string& fileName){
    if(fileName.length()==0)return "";
    int found=fileName.find_last_of('.');
    if(found==-1){
        err("Unknow type");
        return "";
    }
    std::string postfix=fileName.substr(found+1);
    for(int i=0;i<postfix.length();++i){
        if(postfix[i]>='A'&&postfix[i]<='Z')postfix[i]+=32;
    }
    return postfix;
}