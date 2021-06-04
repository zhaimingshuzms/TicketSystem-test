#include <iostream>
#include "usersystem.hpp"
#include "ticketsystem.hpp"
#include "lib/fakebpt.hpp"
#include "lib/time.hpp"
#include <bits/stdc++.h>
/*class ftrain {
public:
    MYSTR<21> trainID;
    UINT stationNum;
    UINT seatNum;
    MYSTR<31> stations[100];
    UINT prices[100];
    OTime startOTime;
    OTime travelTimes[100];
    OTime stopoverTimes[100];
    Date saleDate_b, saleDate_e;
    CHA type;
    //my reserved area
    UINT sellday;
    //UINT resttickets[100][100];
    OTime arrivalTimes[100];
    OTime leavingTimes[100];
    UINT priceprefix[100];
    Time startTime;
    ftrain(){
    }
};*/
int main() {
    //std::cerr << "Hello, World!µÔÃ÷Êæ" << std::endl;
    //FakeBpt<int,int> mp("test.txt");
    //mp.insert(std::make_pair(1,2));
    //mp.insert(std::make_pair(2,3));
    //std::cout<<mp[1]<<" "<<mp[2]<<std::endl;
    //freopen("./data/basic_2/1.in","r",stdin);
    //freopen("T.out","w",stdout);
    STR input;
    ticketsystem s;
    int cnt=0;
    while (getline(std::cin,input)){
        ++cnt;
        //std::cerr<<"cnt"<<cnt<<std::endl;
        /*if (cnt==806) {
            input += " -debug fuck";
            std::cerr << "input---------" << input << '\n';
        }*/
        if (!s.respond(input)) break;
    }
    return 0;
}
