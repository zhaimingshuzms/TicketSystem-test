//
// Created by Administrator on 2021/4/30.
//

#ifndef TICKETSYSTEM_INPUT_TRAITS_HPP
#define TICKETSYSTEM_INPUT_TRAITS_HPP
#include <string>
#include "./lib/map.hpp"
typedef std::string STR;
typedef STR::const_iterator STRITER;
typedef std::size_t UINT;
class input_traits{
    STR str;
    STRITER it;
public:
    input_traits(STR s):str(s),it(str.cbegin()){
    }
    STR get(char c=' '){
        while (it != str.cend() && (*it) == c) ++it;
        STR ret;
        while (it != str.cend() && (*it) != c){
            ret += (*(it++));
        }
        return ret;
    }
    friend std::ostream& operator <<(std::ostream & os,const input_traits &in){
        return os<<in.str;
    }
};
class parse{
    bool alive;
    sjtu::map<STR,STR> mp;
public:
    parse(STR str){
        alive=1;//no exception
        input_traits in(str);
        mp["-or"]=in.get();
        STR tmp;
        while ((tmp=in.get())!="") {
            STR tmp2=in.get();
            mp[tmp]=tmp2;
        }
    }
    const STR& operator[](const STR &str) const{
        return mp.find(str)->second;
    }
    size_t count(const STR &str) const{
        return mp.count(str);
    }
};
#endif //TICKETSYSTEM_INPUT_TRAITS_HPP
