//
// Created by Administrator on 2021/5/15.
//

#ifndef TICKETSYSTEM_TICKETINNERSYSTEM_HPP
#define TICKETSYSTEM_TICKETINNERSYSTEM_HPP
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "lib/time.hpp"
#include "lib/mystring.hpp"
#include "lib/algorithm.hpp"
#include "lib/vector.hpp"
#include "trainsystem.hpp"
#include "BPlusTree.h"
#include <map>
#include "lib/fakebpt.hpp"
#include <unordered_map>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>
using namespace __gnu_pbds;
typedef std::string STR;
typedef unsigned long long ULL;
struct traininfo{
    UINT trainID;
    Date date_b,date_e;
    UINT sid;
    traininfo(){
    }
    traininfo(const UINT &s,const Date &d1,const Date &d2,const UINT &u):trainID(s),date_b(d1),date_e(d2),sid(u){
    }
};
struct order{
    MYSTR<21> userID;
    UINT trainID;
    UINT dayID;
    UINT sid,tid;
    UINT num;
    UINT stat;//0 pending 1 success 2 refunded
    order(){
    }
    order(const MYSTR<21> &s1,const UINT &s2,UINT s3,UINT s4,UINT s5,UINT s6,UINT s7):userID(s1),trainID(s2),dayID(s3),sid(s4),tid(s5),num(s6),stat(s7){
    }
};
struct exact_train{
    UINT trainID;
    UINT DayID;
    exact_train(){
    }
    exact_train(const UINT &s1,UINT s2):trainID(s1),DayID(s2){
    }
    bool operator <(const exact_train &tr) const{
        return trainID<tr.trainID||(trainID==tr.trainID&&DayID<tr.DayID);
    }
    bool operator ==(const exact_train &tr) const{
        return trainID==tr.trainID&&DayID==tr.DayID;
    }
};
struct firsttraininfo{
    UINT trainID;
    OTime stime;
    OTime mtime;
    UINT price;
    UINT seat;
    firsttraininfo(const UINT &s1,const OTime &s2,const OTime &s3,const UINT &s4,const UINT &s5):trainID(s1),stime(s2),mtime(s3),price(s4),seat(s5){
    }
    firsttraininfo(){
    }
};
sjtu::map<ULL,sjtu::vector<firsttraininfo> > mp;
class ticketinnersystem{
    static const int TRAINNUM=100000;//2 times too large
    trainsystem * pts;
    BPlusTree<std::pair<ULL,UINT>,traininfo> c;//multimap hai mei gai
    BPlusTree<std::pair<ULL,UINT>,UINT> d;
    BPlusTree<std::pair<MYSTR<21>,UINT>,order> orderlist;
    BPlusTree<std::pair<exact_train,UINT>,order> pendingqueue;
    ticketinfo *vr;
    UINT vrsize;
    UINT trainnum;
    UINT ordernum;
    STR stat[3]={"pending","success","refunded"};
public:
    ticketinnersystem(trainsystem *p):pts(p),c("c.bin"),d("d.bin"),orderlist("orderlist.bin"),pendingqueue("pendingqueue.bin"){
        vr=new ticketinfo[TRAINNUM];
        std::ifstream infile("num.bin",std::ios_base::in|std::ios_base::binary);
        if (!infile.is_open()) trainnum=0,ordernum=0;
        else{
            infile.read(reinterpret_cast<char *>(&trainnum),sizeof(trainnum));
            infile.read(reinterpret_cast<char *>(&ordernum),sizeof(ordernum));
        }
        infile.close();
    }
    ~ticketinnersystem(){
        std::ofstream outfile("num.bin",std::ios_base::out|std::ios_base::binary);
        outfile.write(reinterpret_cast<const char *>(&trainnum),sizeof(trainnum));
        outfile.write(reinterpret_cast<const char *>(&ordernum),sizeof(ordernum));
        outfile.close();
        delete vr;
    }
    bool release_train(const parse &in){
        ++trainnum;
        auto tmp=pts->trainname2.find(in["-i"]);
        if (!pts->con[tmp.first].released&&tmp.second){
            train &&t=pts->con[tmp.first];
            for (UINT i=0; i<t.stationNum-1; ++i){
                c.insert(std::make_pair(std::make_pair(t.stationhash[i],trainnum),traininfo(t.trainind,t.leavingtime(i,0).date,
                                                                                         t.leavingtime(i,t.saleDate_e-t.saleDate_b).date,i)));
                d.insert(std::make_pair(std::make_pair(t.stationhash[i+1],trainnum),t.trainind));
            }
            t.released=true;
            pts->con.update(tmp.first,t);
            return true;
        }
        return false;
    }
    bool query_ticket(const parse &in){
        //if (in.count("-debug")) std::cerr<<in["-s"]<<" "<<in["-t"]<<" "<<in["-d"]<<" "<<in["-p"]<<" "<<c.size()<<std::endl;
        auto ins=myhash(in["-s"]),intt=myhash(in["-t"]);
        auto pr=c.range_find(std::make_pair(ins,0),std::make_pair(ins,trainnum));

        vrsize=0;
        for (auto &i:pr) {
            //if (in.count("-debug")&&i.trainID==MYSTR<20>("imperiouswaves")) std::cerr<<i.trainID<<" "<<i.date_b<<" "<<i.date_e<<std::endl;
            if (i.second.date_b <= in["-d"] && Date(in["-d"]) <= i.second.date_e) {//maybeslow
                train &&t=pts->con[i.second.trainID];
                UINT tmp=t.findstation(intt);
                if (tmp==STATION_NUM||tmp<i.second.sid) continue;
                vr[vrsize++] = pts->query_ticket(t,i.second.trainID, i.second.sid, tmp, in["-d"]);
            }
        }
        mysort(vr,vr+vrsize,(in.count("-p")&&in["-p"]=="time")?[](ticketinfo &x,ticketinfo &y){
            return (x.t2-x.t1<y.t2-y.t1)||(x.t2-x.t1==y.t2-y.t1&&trainname[x.trainID]<trainname[y.trainID]);//care
        }:[](ticketinfo &x,ticketinfo &y){
            return x.price<y.price||(x.price==y.price&&trainname[x.trainID]<trainname[y.trainID]);//care
        });
        std::cout<<vrsize<<'\n';
        for (UINT i=0; i<vrsize; ++i) std::cout<<trainname[vr[i].trainID]<<" "<<in["-s"]<<" "<<vr[i].t1<<" "<<"->"<<" "<<in["-t"]<<" "<<vr[i].t2<<" "<<
        vr[i].price<<" "<<vr[i].seat<<'\n';
        return true;
    }
    bool query_transfer(const parse &in){//neng bu neng hai shi tong yi liang che
        //an suo xu shi jian pai
        //std::cerr<<"query_transfer"<<in["-s"]<<" "<<in["-t"]<<std::endl;
        ULL ins=myhash(in["-s"]),intt=myhash(in["-t"]);
        //if (in.count("-debug")) std::cerr<<ins<<" "<<intt<<std::endl;
        auto pr=c.range_find(std::make_pair(ins,0),std::make_pair(ins,trainnum));
        // fast fast fast fast
        mp.clear();
        for (auto &i:pr)
            if (i.second.date_b <= in["-d"] && Date(in["-d"]) <= i.second.date_e) {
                train &&t=pts->con[i.second.trainID];
                UINT sid=i.second.sid,dayID=t.DayID(sid,in["-d"]);
                if (!in.count("-p")||in["-p"]=="time"){
                    for (UINT i=sid+1; i<t.stationNum; ++i)
                        mp[t.stationhash[i]].push_back(firsttraininfo(t.trainind,t.leavingtime(sid,dayID),t.arrivetime(i,dayID),t.price(sid,i),t.seat(sid,i,dayID)));
                }
                else{
                    for (UINT i=sid+1; i<t.stationNum; ++i)
                        mp[t.stationhash[i]].push_back(firsttraininfo(t.trainind,t.leavingtime(sid,dayID),t.arrivetime(i,dayID),t.price(sid,i),t.seat(sid,i,dayID)));
                }
            }
        auto pv=d.range_find(std::make_pair(intt,0),std::make_pair(intt,trainnum));
        UINT minn=1e9,minn2=1e9;
        std::ostringstream ss;
        for (UINT i=0; i<pv.size(); ++i){
            train &&t=pts->con[pv[i].second];
            //if (in.count("-debug")) std::cerr<<t.trainID<<std::endl;
            UINT tid=t.findstation(intt);
            for (UINT i=0; i<tid; ++i) {
                auto tmp = mp.find(t.stationhash[i]);
                if (tmp==mp.end()) continue;
                for (auto pp = tmp->second.begin(); pp != tmp->second.end(); ++pp) {
                    auto p=*pp;
                    if (p.trainID == t.trainind) continue;
                    UINT lim = t.saleDate_e - t.saleDate_b;
                    for (UINT j = 0; j <= lim; ++j)
                        if (OTime(t.leavingtime(i, j)) >= p.mtime) {
                            //if (in.count("-debug")) std::cerr<<"J"<<j<<" "<<t.price(i,tid)<<std::endl;
                            auto update = [&]() {
                                ss.str("");
                                ss << trainname[p.trainID] << " ";
                                ss << in["-s"] << " ";
                                ss << p.stime << " ";
                                ss << "-> ";
                                ss << stations[t.stationhash[i]] << " ";
                                ss << p.mtime << " ";
                                ss << p.price << " ";
                                ss << p.seat << "\n";
                                ss << trainname[t.trainind] << " ";
                                ss << stations[t.stationhash[i]] << " ";
                                ss << t.leavingtime(i, j) << " ";
                                ss << "-> ";
                                ss << in["-t"] << " ";
                                ss << t.arrivetime(tid, j) << " ";
                                ss << t.price(i, tid) << " ";
                                ss << t.seat(i, tid, j) << "\n";
                            };
                            if (!in.count("-p") || in["-p"] == "time") {
                                if ((t.arrivetime(tid, j) - p.stime) < minn ||
                                    (t.arrivetime(tid, j) - p.stime) == minn && p.mtime - p.stime < minn2) {
                                    minn = (t.arrivetime(tid, j) - p.stime).pass;
                                    minn2 = (p.mtime - p.stime).pass;
                                    update();
                                }
                            } else {
                                if (t.price(i, tid) + p.price < minn ||
                                    (t.price(i, tid) + p.price == minn && p.mtime - p.stime < minn2)) {
                                    minn = t.price(i, tid) + p.price;
                                    minn2 = (p.mtime - p.stime).pass;
                                    update();
                                }
                            }
                            break;
                        }
                }
            }
        }
        if (minn<1e9) std::cout << ss.str(); else std::cout<<0<<'\n';
        return true;
    }
    bool satisfied(train &t,const order &o){
        return t.seat(o.sid,o.tid,o.dayID)>=o.num;
    }
    bool buy_ticket(const parse &in){
        //if (in.count("-debug")) std::cerr<<in["-i"]<<std::endl;
        auto tmp=pts->trainname2[in["-i"]];
        //if (in.count("-debug")) std::cerr<<"tmp"<<tmp<<std::endl;
        train &&t=pts->con[tmp];
        //if (in.count("-debug")) std::cerr<<trainname[t.trainind]<<" "<<t.released<<" "<<t.startTime<<std::endl;
        //if (in.count("-debug")) std::cerr<<"???"<<std::endl;
        if (!t.released) return false;
        //if (in.count("-debug")) std::cerr<<"!!!"<<std::endl;
        UINT sid=t.findstation(myhash(in["-f"])),tid=t.findstation(myhash(in["-t"]));
        if (sid>=tid||sid==STATION_NUM||tid==STATION_NUM) return false;
        if (!t.inrange(sid,in["-d"])) return false;
        UINT DayID=t.DayID(sid,in["-d"]);
        UINT required=strtonum(in["-n"]);
        if (required>t.seatNum) return false;//6.2
        order o(in["-u"],tmp,DayID,sid,tid,required,1);
        //if (in.count("-debug")) std::cerr<<t.seat(sid,tid,DayID)<<std::endl;
        if (satisfied(t,o)){
            t.buy(sid,tid,DayID,required);
            //pts->con.update(in["-i"],t);
            orderlist.insert(std::make_pair(std::make_pair(in["-u"],++ordernum),o));
            std::cout<<(unsigned long long)t.price(sid,tid)*o.num<<'\n';
            return true;
        }
        if (!in.count("-q")||in["-q"]=="false") return false;
        o.stat=0;
        orderlist.insert(std::make_pair(std::make_pair(in["-u"],++ordernum),o));
        pendingqueue.insert(std::make_pair(std::make_pair(exact_train(tmp,DayID),ordernum),o));
        std::cout<<"queue"<<'\n';
        return true;
    }
    bool refund_ticket(const parse &in){
        UINT num;
        if (!in.count("-n")) num=1; else num=strtonum(in["-n"]);
        auto pr=orderlist.range_find(std::make_pair(in["-u"],0),std::make_pair(in["-u"],ordernum));
        if (num>pr.size()) return false;
        order &o=pr[pr.size()-num].second;//dao le
        if (o.stat==2) return false;
        train &&t=pts->con[o.trainID];
        if (o.stat==0){
            pendingqueue.erase(std::make_pair(exact_train(o.trainID,o.dayID),pr[pr.size()-num].first.second));
            o.stat=2;
            orderlist.update(std::make_pair(o.userID,pr[pr.size()-num].first.second),o);//refund
            return true;
        }
        t.refund(o.sid,o.tid,o.dayID,o.num);
        o.stat=2;
        orderlist.update(std::make_pair(o.userID,pr[pr.size()-num].first.second),o);//refund
        auto pd=pendingqueue.range_find(std::make_pair(exact_train(o.trainID,o.dayID),0),std::make_pair(exact_train(o.trainID,o.dayID),ordernum));
        for (auto i=0; i<pd.size(); ++i) {
            order &o=pd[i].second;
            if (satisfied(t, o)) {
                t.buy(o.sid,o.tid,o.dayID,o.num);
                pendingqueue.erase(pd[i].first);
                o.stat=1;
                orderlist.update(std::make_pair(o.userID,pd[i].first.second),o);
            }
        }
        //pts->con.update(o.trainID,t);
        return true;
    }
    bool query_order(const parse &in){
        //std::cerr<<"orderlist"<<orderlist.size()<<" "<<ordernum<<std::endl;
        auto pr=orderlist.range_find(std::make_pair(in["-u"],0),std::make_pair(in["-u"],ordernum));
        std::cout<<pr.size()<<std::endl;
         for (auto i=(int)(pr.size())-1; i>=0; --i) {
            auto &tmp=pr[i].second;
            std::cout << "[" << stat[tmp.stat] << "] ";
            train &&t=pts->con[tmp.trainID];
            std::cout<<trainname[t.trainind]<<" ";
            std::cout<<stations[t.stationhash[tmp.sid]]<<" ";
            std::cout<<t.leavingtime(tmp.sid,tmp.dayID)<<" ";
            std::cout<<"-> ";
            std::cout<<stations[t.stationhash[tmp.tid]]<<" ";
            std::cout<<t.arrivetime(tmp.tid,tmp.dayID)<<" ";
            std::cout<<t.price(tmp.sid,tmp.tid)<<" ";
            std::cout<<tmp.num<<'\n';
        }
        return true;
    }
};
#endif //TICKETSYSTEM_TICKETINNERSYSTEM_HPP
