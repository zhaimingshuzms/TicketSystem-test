//
// Created by Administrator on 2021/5/28.
//

#ifndef TICKETSYSTEM_FAKEBPT_HPP
#define TICKETSYSTEM_FAKEBPT_HPP
#include <map>
#include <iostream>
#include <fstream>
#include "vector.hpp"
/*
template<class T1,class T2>
class FakeBpt{
    //zhao zhe zhe ge xie jie kou
    std::map<T1,T2> mp;
public:
    sjtu::vector<std::pair<T1,T2> > range_find(const T1 &l,const T1 &r){
        auto p1=mp.lower_bound(l),p2=mp.upper_bound(r);
        sjtu::vector<std::pair<T1,T2> > ret;
        for (auto i=p1; i!=p2; ++i) ret.push_back(*i);
        return ret;
    }
    void insert(const std::pair<T1,T2> &x){
        mp.insert(x);
    }
    void erase(const T1 &x){
        mp.erase(x);
    }
    bool empty(){
        return mp.empty();
    }
    int size(){
        return mp.size();
    }
    std::pair<T2,bool> find(const T1 &key) const{
        if (mp.find(key)!=mp.end()) return std::make_pair(mp.find(key)->second,true);
        return std::make_pair(T2(),false);
    }
    size_t count(const T1 &key) const{
        return find(key).second;
    }
    T2 operator [](const T1 &key) const{
        return find(key).first;
    }
    void update(const T1 &x,const T2 &y){
        mp[x]=y;
    }
    void clear(){
        mp.clear();
    }
};*/
template<class T1,class T2>
class FakeBpt{
    //zhao zhe zhe ge xie jie kou
public:
    std::map<T1,T2> mp;
    std::string fname;
    explicit FakeBpt(const std::string &filename):fname(filename){
        std::ifstream infile(filename,std::ios_base::in);
        if (!infile.is_open()){
            std::ofstream outfile(filename,std::ios_base::out|std::ios_base::binary);
            outfile.close();
        }
        infile.close();
        std::fstream file(filename,std::ios_base::in|std::ios_base::out|std::ios_base::binary);
        T1 tmp1;
        T2 tmp2;
        //tellg tellp
        int length=file.seekg(0,std::ios_base::end).tellg();
        file.seekg(0,std::ios_base::beg);
        //std::cerr<<"length"<<length<<std::endl;
        while (file.tellg()<length){
            //std::cerr<<"file "<<file.tellg()<<" "<<sizeof(tmp1)<<std::endl;
            file.read(reinterpret_cast<char *>(&tmp1),sizeof(tmp1));
            //file.seekg(sizeof(tmp1),std::ios_base::cur);
            file.read(reinterpret_cast<char *>(&tmp2),sizeof(tmp2));
            //file.seekg(sizeof(tmp2),std::ios_base::cur);
            mp[tmp1]=tmp2;
        }
        file.close();
        //std::cerr<<"construct end"<<std::endl;
    }
    ~FakeBpt(){
        std::ofstream outfile(fname,std::ios_base::out|std::ios_base::binary);
        outfile.seekp(0,std::ios_base::beg);
        for (auto i:mp){
            outfile.write(reinterpret_cast<const char *>(&i.first),sizeof(i.first));
            //outfile.seekp(sizeof(i.first),std::ios_base::cur);
            outfile.write(reinterpret_cast<const char *>(&i.second),sizeof(i.second));
            //outfile.seekp(sizeof(i.second),std::ios_base::cur);
        }
        outfile.close();
    }
    sjtu::vector<std::pair<T1,T2> > range_find(const T1 &l,const T1 &r){
        auto p1=mp.lower_bound(l),p2=mp.upper_bound(r);
        sjtu::vector<std::pair<T1,T2> > ret;
        for (auto i=p1; i!=p2; ++i) ret.push_back(*i);
        return ret;
    }
    void insert(const std::pair<T1,T2> &x){
        mp.insert(x);
    }
    void erase(const T1 &x){
        mp.erase(x);
    }
    bool empty(){
        return mp.empty();
    }
    int size(){
        return mp.size();
    }
    std::pair<T2,bool> find(const T1 &key) const{
        if (mp.find(key)!=mp.end()) return std::make_pair(mp.find(key)->second,true);
        return std::make_pair(T2(),false);
    }
    size_t count(const T1 &key) const{
        return find(key).second;
    }
    T2 operator [](const T1 &key) const{
        return find(key).first;
    }
    void update(const T1 &x,const T2 &y){
        mp[x]=y;
    }
    void clear(){
        mp.clear();
    }
};

#endif //TICKETSYSTEM_FAKEBPT_HPP
