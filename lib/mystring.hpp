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
public:
    UINT top;
    char ch[N];
    MYSTR(){
        memset(ch,0,sizeof(ch));
        top=0;
    }
    MYSTR(const STR &ss){//?????
        auto s=ss.c_str();
        memset(ch,0,sizeof(ch));
        top=0;
        for (UINT i=0; s[i]; ++i) ch[top++]=s[i];
    }
    MYSTR(const char * s){
        memset(ch,0,sizeof(ch));
        top=0;
        for (UINT i=0; s[i]; ++i) ch[top++]=s[i];
    }
    operator std::string(){
        return std::string(ch);
    }
    char & operator [](const UINT &pos){
        //if (pos>=N) throw;
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
    bool operator >=(const MYSTR & s) const{
        for (UINT i=0; i<top&&i<s.top; ++i)
            if (ch[i]!=s.ch[i]) return ch[i]>s.ch[i];
        return top>=s.top;
    }
    bool operator <=(const MYSTR & s) const{
        for (UINT i=0; i<top&&i<s.top; ++i)
            if (ch[i]!=s.ch[i]) return ch[i]<s.ch[i];
        return top<=s.top;
    }
};
#endif //TICKETSYSTEM_MYSTRING_HPP
