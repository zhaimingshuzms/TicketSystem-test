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
typedef unsigned long long ULL;
const int DAY_NUM=92;
const int STATION_NUM=100;
std::fstream p;
UINT aval[STATION_NUM];
static BPlusTree<ULL,MYSTR<31> >stations("stations.bin");
void saveticket(int trainind,int dayid,int lid,int rid,UINT val){
    int offset=trainind*DAY_NUM*STATION_NUM+dayid*STATION_NUM+lid;
    p.seekp(offset*sizeof(UINT),ios_base::beg);
    for (UINT i=0; i<=rid-lid; ++i) aval[i]=val;
    p.write(reinterpret_cast<const char *>(&aval),sizeof(*aval)*(rid-lid+1));
}
void saveticket(int trainind,int dayid,int id,UINT val){
    int offset=trainind*DAY_NUM*STATION_NUM+dayid*STATION_NUM+id;
    p.seekp(offset*sizeof(UINT),ios_base::beg);
    p.write(reinterpret_cast<const char *>(&val),sizeof(val));
}
UINT findticket(int trainind,int dayid,int id){
    int offset=trainind*DAY_NUM*STATION_NUM+dayid*STATION_NUM+id;
    p.seekg(offset*sizeof(UINT),ios::beg);
    UINT val;
    p.read(reinterpret_cast<char *>(&val),sizeof(val));
    return val;
}
UINT findticketmin(int trainind,int dayid,int lid,int rid){
    int offset=trainind*DAY_NUM*STATION_NUM+dayid*STATION_NUM+lid;
    p.seekg(offset*sizeof(UINT),ios::beg);
    p.read(reinterpret_cast<char *>(&aval), sizeof(*aval)*(rid-lid+1));
    UINT ret=1e9;
    for (UINT i=0; i<=rid-lid; ++i)
        ret=std::min(ret,aval[i]);
    return ret;
}
OTime stopoverTimes[STATION_NUM];
OTime travelTimes[STATION_NUM];
UINT prices[STATION_NUM];
class train{
public:
    UINT stationNum;
    UINT seatNum;
    ULL stationhash[STATION_NUM];
    OTime startOTime;
    Date saleDate_b,saleDate_e;
    CHA type;
    //my reserved area
    UINT sellday;
    //UINT resttickets[DAY_NUM][STATION_NUM];
    UINT trainind;
    OTime arrivalTimes[STATION_NUM];
    OTime leavingTimes[STATION_NUM];
    UINT priceprefix[STATION_NUM];
    Time startTime;
    bool released;
    train(){
    }
    train(const UINT &s2,const UINT &s3,const STR &s4,
          const STR &s5,const STR &s6,const STR &s7,const STR &s8,const STR &s9,const STR &s10,const UINT &s11){
        stationNum=s2;
        seatNum=s3;
        sjtu::vector<STR> v=divide(s4,'|');
        //for (UINT i=0; i<stationNum; ++i) stations[i]=v[i];
        for (UINT i=0; i<stationNum; ++i) {
            stationhash[i] = myhash(v[i]);
            if (!stations.count(stationhash[i])) stations.insert(stationhash[i],v[i]);
        }
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

        trainind=s11;
        for (UINT i=0; i<sellday; ++i)
                saveticket(trainind,i,0,stationNum-1,seatNum);

        arrivalTimes[0]=0;
        leavingTimes[0]=0;
        for (UINT i=1; i<stationNum; ++i){
            arrivalTimes[i]=leavingTimes[i-1]+travelTimes[i];
            if (i<stationNum-1) leavingTimes[i]=arrivalTimes[i]+stopoverTimes[i];//????
        }
        priceprefix[0]=0;
        for (UINT i=1; i<stationNum; ++i) priceprefix[i]=priceprefix[i-1]+prices[i];
        startTime=startOTime;
        startTime.date=saleDate_b;
        released=false;
        //std::cout<<startTime<<" "<<saleDate_b<<" "<<saleDate_e<<" "<<saleDate_e-saleDate_e<<std::endl;
    }
    bool query(BPlusTree<UINT,MYSTR<21> > &trainname,std::ostream &os,const Date &d){
        if (d<saleDate_b||saleDate_e<d) return false;
        os<<trainname[trainind]<<" "<<type<<'\n';
        Time st=startTime;
        st.date=d;
        for (UINT i=0; i<stationNum; ++i) {
            os << stations[stationhash[i]]<<" ";
            if (i == 0) os << "xx-xx xx:xx "; else os << st + arrivalTimes[i] << " ";
            os << "-> ";
            if (i < stationNum - 1) os << st + leavingTimes[i] <<" "; else os << "xx-xx xx:xx ";
            os << priceprefix[i]<<" ";
            if (i < stationNum - 1) os << findticket(trainind,d - saleDate_b,i+1); else os << "x";
            os << '\n';
        }
        return true;
    }
    /*UINT findstation(const MYSTR<31> &s){
        for (UINT i=0; i<stationNum; ++i)
            if (stations[i]==s) return i;
        return STATION_NUM;
    }*/
    UINT findstation(const ULL &s){
        for (UINT i=0; i<stationNum; ++i)
            if (stationhash[i]==s) return i;
        return STATION_NUM;
    }
    UINT price(const UINT &sid,const UINT &tid){
        return priceprefix[tid]-priceprefix[sid];
    }
    UINT seat(const UINT &sid,const UINT &tid,const UINT &dayID){
        return findticketmin(trainind,dayID,sid+1,tid);
    }
    void buy(const UINT &sid,const UINT &tid,const UINT &dayID,const UINT &num){
        for (UINT i=sid+1; i<=tid; ++i)
            saveticket(trainind,dayID,i,findticket(trainind,dayID,i)-num);
    }
    void refund(const UINT &sid,const UINT &tid,const UINT &dayID,const UINT &num){
        for (UINT i=sid+1; i<=tid; ++i)
            saveticket(trainind,dayID,i,findticket(trainind,dayID,i)+num);
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
    UINT trainID;
    OTime t1,t2;
    UINT seat,price;
};
static BPlusTree<UINT,MYSTR<21> > trainname("trainname.bin");
class ticketinnersystem;
class trainsystem{
    friend class ticketinnersystem;
    //BPlusTree<UINT,bool> list;//modified
    BPlusTree<UINT,train,50> con;//ji de 10
    BPlusTree<MYSTR<21>,UINT> trainname2;
    UINT trainind;
public:
    trainsystem():con("train.bin"),trainname2("trainname2.bin"){
        p.open("ticket.bin",std::ios_base::out|std::ios_base::in|std::ios_base::binary);
        if (!p) p.open("ticket.bin",std::ios_base::out|std::ios_base::binary);
        p.close();
        p.open("ticket.bin",std::ios_base::out|std::ios_base::in|std::ios_base::binary);

        std::ifstream infile("num.bin",std::ios_base::in|std::ios_base::binary);
        if (!infile.is_open()) trainind=0;
        else{
            infile.seekg(sizeof(UINT)*2,ios::beg);
            infile.read(reinterpret_cast<char *>(&trainind),sizeof(trainind));
        }
        infile.close();
    }
    ~trainsystem(){
        std::ofstream outfile("num.bin",std::ios_base::in|std::ios_base::out|std::ios_base::binary);
        outfile.seekp(sizeof(UINT)*2,ios::beg);
        outfile.write(reinterpret_cast<const char *>(&trainind),sizeof(trainind));
        outfile.close();
        p.close();
    }
    //hao xiang zi sha
    bool add_train(const parse &in){
        if (trainname2.count(in["-i"])) return false;
        trainname.insert(std::make_pair(trainind,in["-i"]));
        trainname2.insert(std::make_pair(in["-i"],trainind));
        train t(strtonum(in["-n"]),strtonum(in["-m"]),in["-s"],in["-p"],in["-x"],in["-t"],in["-o"],in["-d"],in["-y"],trainind);
        con.insert(std::make_pair(trainind,t));
        ++trainind;
        return true;
    }
    bool delete_train(const parse &in){
        auto tmp=trainname2.find(in["-i"]);
        if (!tmp.second) return false;
        if (con[tmp.first].released) return false;
        con.erase(tmp.first);
        trainname.erase(tmp.first);
        trainname2.erase(in["-i"]);
        //std::cerr<<"success"<<std::endl;
        return true;
    }
    bool query_train(const parse &in){
        auto tmp=trainname2.find(in["-i"]);
        if (!tmp.second) return false;
        return con[tmp.first].query(trainname,std::cout,in["-d"]);
    }
    ticketinfo query_ticket(const UINT &trainID,const ULL &s,const ULL &t,const Date &d){
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
