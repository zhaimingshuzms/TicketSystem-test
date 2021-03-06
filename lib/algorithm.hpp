//
// Created by Administrator on 2021/5/12.
//

#ifndef TICKETSYSTEM_ALGORITHM_HPP
#define TICKETSYSTEM_ALGORITHM_HPP
#include "vector.hpp"
#include <string>
typedef std::string STR;
typedef unsigned char CHA;
sjtu::vector<STR> divide(const STR &s,const CHA &ch){
    STR ss;
    sjtu::vector<STR> v;
    for (auto i:s)
        if (i==ch){
            v.push_back(ss);
            ss.clear();
        }
        else ss+=i;
    if (ss.size()) v.push_back(ss);
    return v;
}
template<class T>
void qsort(T* be,T *ta,bool (*f)(T &,T &)){
    T mid=*(be+(ta-be)/2);
    T *p1=be,*p2=ta;
    do{
        while (f(*p1,mid)) p1++;
        while (f(mid,*p2)) p2--;
        if (p1<=p2){
            std::swap(*p1,*p2);
            ++p1;
            --p2;
        }
    }while (p1<=p2);
    if (be<p2) qsort(be,p2,f);
    if (p1<ta) qsort(p1,ta,f);
}
template<class T>
void mysort(T* be,T* en,bool (*f)(T &,T &)){
    //need to be done
    if (be<en) qsort(be,--en,f);
}
const unsigned long long A=317;
const unsigned long long B=10000000000000000ull+9;
unsigned long long myhash(const MYSTR<31> &s){
    unsigned long long ret=0;
    for (UINT i=0; s.ch[i]; ++i) ret=(ret*A+s.ch[i])%B;
    return ret;
}
unsigned long long myhash2(const MYSTR<21> &s){
    unsigned long long ret=0;
    for (UINT i=0; s.ch[i]; ++i) ret=(ret*A+s.ch[i])%B;
    return ret;
}
#endif //TICKETSYSTEM_ALGORITHM_HPP
