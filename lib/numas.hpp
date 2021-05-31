//
// Created by Administrator on 2021/5/2.


#ifndef TICKETSYSTEM_NUMAS_HPP
#define TICKETSYSTEM_NUMAS_HPP
#include <string>
unsigned int strtonum(const std::string &str){
    unsigned int ret=0;
    for (auto i:str) ret=ret*10+(i-'0');
    return ret;
}
#endif //TICKETSYSTEM_NUMAS_HPP
