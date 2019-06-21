#include "utils.h"

void err(const std::string& str){
    perror(str.c_str());
}