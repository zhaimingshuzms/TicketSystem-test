#include <iostream>
#include "usersystem.hpp"
#include "ticketsystem.hpp"
#include "lib/fakebpt.hpp"
#include "lib/time.hpp"
#include <bits/stdc++.h>
int main() {
    //std::cerr << "Hello, World!µÔÃ÷Êæ" << std::endl;
    //FakeBpt<int,int> mp("test.txt");
    //mp.insert(std::make_pair(1,2));
    //mp.insert(std::make_pair(2,3));
    //std::cout<<mp[1]<<" "<<mp[2]<<std::endl;
    //freopen("./data/basic_1/1.in","r",stdin);
    //freopen("T.out","w",stdout);
    STR input;
    ticketsystem s;
    int cnt=0;
    while (getline(std::cin,input)){
        ++cnt;
        //std::cerr<<"cnt"<<cnt<<std::endl;
        /*(if (cnt==1453) {
            input += " -debug fuck";
            std::cerr << "input---------" << input << '\n';
        }*/
        if (!s.respond(input)) break;
    }
    return 0;
}
