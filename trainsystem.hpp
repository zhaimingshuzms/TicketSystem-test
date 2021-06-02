//
// Created by Administrator on 2021/5/12.
//

#ifndef TICKETSYSTEM_TRAINSYSTEM_HPP
#define TICKETSYSTEM_TRAINSYSTEM_HPP
#include <string>
#include "lib/mystring.hpp"
#include "lib/numas.hpp"
#include "lib/time.hpp"
#include "lib/vector.hpp"
#include "lib/algorithm.hpp"
#include "input_traits.hpp"
#include "BPlusTree.h"
#include <map>
typedef std::string STR;
typedef unsigned char CHA;
const int DAY_NUM=92;
const int STATION_NUM=100;
class train{
public:
    MYSTR<20> trainID;
    UINT stationNum;
    UINT seatNum;
    MYSTR<30> stations[STATION_NUM];
    UINT prices[STATION_NUM];
    OTime startOTime;
    OTime travelTimes[STATION_NUM];
    OTime stopoverTimes[STATION_NUM];
    Date saleDate_b,saleDate_e;
    CHA type;
    //my reserved area
    UINT sellday;
    UINT resttickets[DAY_NUM][STATION_NUM];
    OTime arrivalTimes[STATION_NUM];
    OTime leavingTimes[STATION_NUM];
    UINT priceprefix[STATION_NUM];
    Time startTime;
    train(){
    }
    train(const MYSTR<20> &s1,const UINT &s2,const UINT &s3,const STR &s4,
          const STR &s5,const STR &s6,const STR &s7,const STR &s8,const STR &s9,const STR &s10){
        trainID=s1;
        stationNum=s2;
        seatNum=s3;
        sjtu::vector<STR> v=divide(s4,'|');
        for (UINT i=0; i<stationNum; ++i) stations[i]=v[i];
        v=divide(s5,'|');
        for (UINT i=1; i<stationNum; ++i) prices[i]=strtonum(v[i-1]);
        v=divide(s6,':');
        startOTime=strtonum(v[0])*MIN_PER_HOUR+strtonum(v[1]);
        v=divide(s7,'|');
        for (UINT i=1; i<stationNum; ++i) travelTimes[i]=strtonum(v[i-1]);
        v=divide(s8,'|');
        for (UINT i=1; i<stationNum-1; ++i) stopoverTimes[i]=strtonum(v[i-1]);
        v=divide(s9,'|');
        saleDate_b=Date(v[0]);
        saleDate_e=Date(v[1]);
        type=s10[0];
        sellday=UINT(saleDate_e-saleDate_b)+1;
        for (UINT i=0; i<sellday; ++i)
            for (UINT j=0; j<stationNum; ++j)
                resttickets[i][j]=seatNum;
        arrivalTimes[0]=0;
        leavingTimes[0]=0;
        for (UINT i=1; i<stationNum; ++i){
            arrivalTimes[i]=leavingTimes[i-1]+travelTimes[i];
            if (i<stationNum) leavingTimes[i]=arrivalTimes[i]+stopoverTimes[i];//????
        }
        priceprefix[0]=0;
        for (UINT i=1; i<stationNum; ++i) priceprefix[i]=priceprefix[i-1]+prices[i];
        startTime=startOTime;
        startTime.date=saleDate_b;
        //std::cout<<startTime<<" "<<saleDate_b<<" "<<saleDate_e<<" "<<saleDate_e-saleDate_e<<std::endl;
    }
    bool query(std::ostream &os,const Date &d){
        if (d<saleDate_b||saleDate_e<d) return false;
        os<<trainID<<" "<<type<<'\n';
        Time st=startTime;
        st.date=d;
        for (UINT i=0; i<stationNum; ++i) {
            os << stations[i]<<" ";
            if (i == 0) os << "xx-xx xx:xx "; else os << st + arrivalTimes[i] << " ";
            os << "-> ";
            if (i < stationNum - 1) os << st + leavingTimes[i] <<" "; else os << "xx-xx xx:xx ";
            os << priceprefix[i]<<" ";
            if (i < stationNum - 1) os << resttickets[d - saleDate_b][i+1]; else os << "x";
            os << '\n';
        }
        return true;
    }
    UINT findstation(const MYSTR<30> &s){
        for (UINT i=0; i<stationNum; ++i)
            if (stations[i]==s) return i;
        return STATION_NUM;
    }
    UINT price(const UINT &sid,const UINT &tid){
        return priceprefix[tid]-priceprefix[sid];
    }
    UINT seat(const UINT &sid,const UINT &tid,const UINT &dayID){
        UINT ret=seatNum;
        for (UINT i=sid+1; i<=tid; ++i)
            ret=std::min(ret,resttickets[dayID][i]);
        return ret;
    }
    void buy(const UINT &sid,const UINT &tid,const UINT &dayID,const UINT &num){
        for (UINT i=sid+1; i<=tid; ++i)
            resttickets[dayID][i]-=num;
    }
    void refund(const UINT &sid,const UINT &tid,const UINT &dayID,const UINT &num){
        for (UINT i=sid+1; i<=tid; ++i)
            resttickets[dayID][i]+=num;
    }
    UINT DayID(const UINT &id,const Date &d){
        Time t=startTime+leavingTimes[id];
        return d-t.date;
    }
    bool inrange(const UINT &id,const Date &d){
        Time t=startTime+leavingTimes[id];
        return t.date<=d&&d<=(t.date+(saleDate_e-saleDate_b));
    }
    Time arrivetime(const UINT &id,const UINT &dayID){
        return (startTime+OTime(dayID*MIN_PER_DAY))+arrivalTimes[id];
    }
    Time leavingtime(const UINT &id,const UINT &dayID){
        return (startTime+OTime(dayID*MIN_PER_DAY))+leavingTimes[id];
    }
};
struct ticketinfo{
    MYSTR<20> trainID;
    Time t1,t2;
    UINT seat,price;
};
class ticketinnersystem;
class trainsystem{
    friend class ticketinnersystem;
    FakeBpt<MYSTR<20>,bool> list;//modified
    FakeBpt<MYSTR<20>,train> con;
public:
    trainsystem():list("releasetrain.bin"),con("train.bin"){
    }
    //hao xiang zi sha
    bool add_train(const parse &in){
        if (con.count(in["-i"])) return false;
        //std::cerr<<"addsuccess"<<in["-i"]<<std::endl;
        train t(in["-i"],strtonum(in["-n"]),strtonum(in["-m"]),in["-s"],in["-p"],in["-x"],in["-t"],in["-o"],in["-d"],in["-y"]);
        con.insert(std::make_pair(in["-i"],t));
        return true;
    }
    bool delete_train(const parse &in){
        if (!con.count(in["-i"])) return false;
        if (list.count(in["-i"])) return false;
        con.erase(in["-i"]);
        //std::cerr<<"success"<<std::endl;
        return true;
    }
    bool query_train(const parse &in){
        if (!con.count(in["-i"])) return false;
        return con[in["-i"]].query(std::cout,in["-d"]);
    }
    ticketinfo query_ticket(const MYSTR<20> &trainID,const MYSTR<30> &s,const MYSTR<30> &t,const Date &d){
        train tr=con[trainID];
        UINT sid=tr.findstation(s),tid=tr.findstation(t);
        UINT dayid=tr.DayID(sid,d);
        ticketinfo ret;
        ret.trainID=trainID;
        ret.t1=tr.leavingtime(sid,dayid);
        ret.t2=tr.arrivetime(tid,dayid);
        ret.price=tr.price(sid,tid);
        ret.seat=tr.seat(sid,tid,dayid);
        return ret;
    }
};
#endif //TICKETSYSTEM_TRAINSYSTEM_HPP
