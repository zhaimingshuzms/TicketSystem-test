//
// Created by Administrator on 2021/5/12.
//

#ifndef TICKETSYSTEM_MYSTRING_HPP
#define TICKETSYSTEM_MYSTRING_HPP
#include <iostream>
typedef std::string STR;
typedef std::size_t UINT;
template<int N>
class MYSTR{
    UINT top;
    char ch[N];
public:
    MYSTR(){
        memset(ch,0,sizeof(ch));
        top=0;
    }
    MYSTR(const STR &s){
        memset(ch,0,sizeof(ch));
        top=0;
        for (auto i:s) ch[top++]=i;
    }
    char & operator [](const UINT &pos){
        return ch[pos];
    }
    UINT size(){
        return top;
    }
    bool empty(){
        return top>0;
    }
    friend std::ostream & operator <<(std::ostream &os,const MYSTR &_){
        os<<_.ch;
    }
    bool operator ==(const MYSTR & s) const{
        if (top!=s.top) return 0;
        for (UINT i=0; i<top; ++i)
            if (ch[i]!=s.ch[i]) return 0;
        return 1;
    }
    bool operator !=(const MYSTR & s) const{
        if (top!=s.top) return 1;
        for (UINT i=0; i<top; ++i)
            if (ch[i]!=s.ch[i]) return 1;
        return 0;
    }
    bool operator <(const MYSTR & s) const{
        for (UINT i=0; i<top&&i<s.top; ++i)
            if (ch[i]!=s.ch[i]) return ch[i]<s.ch[i];
        return top<s.top;
    }
    bool operator >(const MYSTR & s) const{
        for (UINT i=0; i<top&&i<s.top; ++i)
            if (ch[i]!=s.ch[i]) return ch[i]>s.ch[i];
        return top>s.top;
    }
};
#endif //TICKETSYSTEM_MYSTRING_HPP
