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
typedef std::string STR;
struct traininfo{
    MYSTR<21> trainID;
    Date date_b,date_e;
    traininfo(){
    }
    traininfo(const MYSTR<21> &s,const Date &d1,const Date &d2):trainID(s),date_b(d1),date_e(d2){
    }
};
struct order{
    MYSTR<21> userID;
    MYSTR<21> trainID;
    UINT dayID;
    UINT sid,tid;
    UINT num;
    UINT stat;//0 pending 1 success 2 refunded
    order(){
    }
    order(const MYSTR<21> &s1,const MYSTR<21> &s2,UINT s3,UINT s4,UINT s5,UINT s6,UINT s7):userID(s1),trainID(s2),dayID(s3),sid(s4),tid(s5),num(s6),stat(s7){
    }
};
struct exact_train{
    MYSTR<21> trainID;
    UINT DayID;
    exact_train(){
    }
    exact_train(const MYSTR<21> &s1,UINT s2):trainID(s1),DayID(s2){
    }
    bool operator <(const exact_train &tr) const{
        return trainID<tr.trainID||(trainID==tr.trainID&&DayID<tr.DayID);
    }
    bool operator ==(const exact_train &tr) const{
        return trainID==tr.trainID&&DayID==tr.DayID;
    }
};
struct firsttraininfo{
    MYSTR<21> trainID;
    OTime stime;
    OTime mtime;
    UINT price;
    UINT seat;
    firsttraininfo(const MYSTR<21> &s1,const OTime &s2,const OTime &s3,const UINT &s4,const UINT &s5):trainID(s1),stime(s2),mtime(s3),price(s4),seat(s5){
    }
    firsttraininfo(){
    }
};

class ticketinnersystem{
    static const int TRAINNUM=100000;//2 times too large
    trainsystem * pts;
    FakeBpt<std::pair<MYSTR<31>,UINT>,traininfo> c;//multimap hai mei gai
    FakeBpt<std::pair<MYSTR<31>,UINT>,MYSTR<21> > d;
    FakeBpt<std::pair<MYSTR<21>,UINT>,order> orderlist;
    FakeBpt<std::pair<exact_train,UINT>,order> pendingqueue;
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
       // if (in["-i"]=="imperiouswaves") std::cerr<<trainnum<<std::endl;
        if (!pts->list.count(in["-i"])&&pts->con.count(in["-i"])){
            train t=pts->con[in["-i"]];
            for (UINT i=0; i<t.stationNum-1; ++i){
                //if (in["-i"]=="imperiouswaves") std::cerr<<t.stations[i]<<std::endl;
//                std::cout<<t.startTime<<std::endl;
                //c.insert(std::make_pair(std::make_pair(t.stations[i],trainnum),traininfo(t.trainID,(t.startTime+t.leavingTimes[i]).date,
                //                                                (t.startTime+(t.saleDate_e-t.saleDate_b)*MIN_PER_DAY+t.arrivalTimes[i]).date)));
                c.insert(std::make_pair(std::make_pair(t.stations[i],trainnum),traininfo(t.trainID,t.leavingtime(i,0).date,
                                                                                         t.leavingtime(i,t.saleDate_e-t.saleDate_b).date)));
                d.insert(std::make_pair(std::make_pair(t.stations[i+1],trainnum),t.trainID));
            }
            pts->list.update(in["-i"],true);
//            for (auto i:c) std::cout<<i.second.trainID<<" "<<i.second.date_b<<"¡¡"<<i.second.date_e<<std::endl;
//           std::cerr<<"release "<<in["-i"]<<" success"<<" "<<t.stationNum<<" "<<c.size()<<std::endl;
//            for (auto i:c.mp) std::cerr<<i.first.first<<" "<<i.first.second<<std::endl;
            return true;
        }
        return false;
    }
    bool query_ticket(const parse &in){
        //if (in.count("-debug")) std::cerr<<in["-s"]<<" "<<in["-t"]<<" "<<in["-d"]<<" "<<in["-p"]<<" "<<c.size()<<std::endl;
        sjtu::vector<traininfo> vs;
        auto pr=c.range_find(std::make_pair(in["-s"],0),std::make_pair(in["-s"],trainnum));
        if (pr.size())
            for (auto i=0; i<pr.size(); ++i) {
              //  if (in.count("-debug")&&pr[i].second.trainID==MYSTR<20>("imperiouswaves")) std::cerr<<"succeed"<<std::endl;
                vs.push_back(pr[i].second);
            }

        vrsize=0;
        for (auto i:vs) {
            //if (in.count("-debug")&&i.trainID==MYSTR<20>("imperiouswaves")) std::cerr<<i.trainID<<" "<<i.date_b<<" "<<i.date_e<<std::endl;
            if (i.date_b <= in["-d"] && Date(in["-d"]) <= i.date_e) {//maybeslow
                UINT tmp=pts->con[i.trainID].findstation(in["-t"]);
                if (tmp==STATION_NUM||tmp<pts->con[i.trainID].findstation(in["-s"])) continue;
                vr[vrsize++] = pts->query_ticket(i.trainID, in["-s"], in["-t"], in["-d"]);
            }
        }
        //use std sort
        /*if (in.count("-debug")){
            for (int i=0; i<vrsize; ++i)
                std::cerr<<vr[i].t2-vr[i].t1<<" "<<vr[i].trainID<<std::endl;
        }*/
        mysort(vr,vr+vrsize,(in.count("-p")&&in["-p"]=="time")?[](ticketinfo &x,ticketinfo &y){
            return (x.t2-x.t1<y.t2-y.t1)||(x.t2-x.t1==y.t2-y.t1&&x.trainID<y.trainID);//care
        }:[](ticketinfo &x,ticketinfo &y){
            return x.price<y.price||(x.price==y.price&&x.trainID<y.trainID);//care
        });
        std::cout<<vrsize<<'\n';
        for (UINT i=0; i<vrsize; ++i) std::cout<<vr[i].trainID<<" "<<in["-s"]<<" "<<vr[i].t1<<" "<<"->"<<" "<<in["-t"]<<" "<<vr[i].t2<<" "<<
        vr[i].price<<" "<<vr[i].seat<<'\n';
        return true;
    }
    bool query_transfer(const parse &in){//neng bu neng hai shi tong yi liang che
        //an suo xu shi jian pai
        //std::cerr<<"query_transfer"<<in["-s"]<<" "<<in["-t"]<<std::endl;
        sjtu::vector<traininfo> vs;
        auto pr=c.range_find(std::make_pair(in["-s"],0),std::make_pair(in["-s"],trainnum));
        if (pr.size())
            for (auto i=0; i<pr.size(); ++i)
                vs.push_back(pr[i].second);
        sjtu::map<MYSTR<31>,sjtu::vector<firsttraininfo> > mp;
        for (auto i:vs)
            if (i.date_b <= in["-d"] && Date(in["-d"]) <= i.date_e) {
                train t=pts->con[i.trainID];
                UINT sid=t.findstation(in["-s"]),dayID=t.DayID(sid,in["-d"]);
                for (UINT i=sid+1; i<t.stationNum; ++i){
                    //auto p=mp.find(t.stations[i]);
                    if (!in.count("-p")||in["-p"]=="time"){
//                        if (p==mp.end()||p->second.mtime>t.arrivetime(i,dayID))
                          mp[t.stations[i]].push_back(firsttraininfo(t.trainID,t.leavingtime(sid,dayID),t.arrivetime(i,dayID),t.price(sid,i),t.seat(sid,i,dayID)));
                    }
                    else{
//                        if (p==mp.end()||p->second.price>t.price(sid,i))
                          mp[t.stations[i]].push_back(firsttraininfo(t.trainID,t.leavingtime(sid,dayID),t.arrivetime(i,dayID),t.price(sid,i),t.seat(sid,i,dayID)));
                    }
                }
            }
        auto pv=d.range_find(std::make_pair(in["-t"],0),std::make_pair(in["-t"],trainnum));
        UINT minn=1e9,minn2=1e9;
        std::ostringstream ss;
        for (UINT i=0; i<pv.size(); ++i){
            train t=pts->con[pv[i].second];
            //if (in.count("-debug")) std::cerr<<t.trainID<<std::endl;
            UINT tid=t.findstation(in["-t"]);
            for (UINT i=0; i<tid; ++i) {
                auto tmp = mp.find(t.stations[i]);
                if (tmp == mp.end()) continue;
                for (auto pp = tmp->second.begin(); pp != tmp->second.end(); ++pp) {
                    auto p=*pp;
                    if (p.trainID == t.trainID) continue;
                    UINT lim = t.saleDate_e - t.saleDate_b;
                    for (UINT j = 0; j <= lim; ++j)
                        if (OTime(t.leavingtime(i, j)) >= p.mtime) {
                            //if (in.count("-debug")) std::cerr<<"J"<<j<<" "<<t.price(i,tid)<<std::endl;
                            auto update = [&]() {
                                ss.str("");
                                ss << p.trainID << " ";
                                ss << in["-s"] << " ";
                                ss << p.stime << " ";
                                ss << "-> ";
                                ss << t.stations[i] << " ";
                                ss << p.mtime << " ";
                                ss << p.price << " ";
                                ss << p.seat << "\n";
                                ss << t.trainID << " ";
                                ss << t.stations[i] << " ";
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
        if (!pts->list.count(in["-i"])) return false;
        train t=pts->con[in["-i"]];
        UINT sid=t.findstation(in["-f"]),tid=t.findstation(in["-t"]);
        if (sid>=tid||sid==STATION_NUM||tid==STATION_NUM) return false;
        if (!t.inrange(sid,in["-d"])) return false;
        UINT DayID=t.DayID(sid,in["-d"]);
        UINT required=strtonum(in["-n"]);
        if (required>t.seatNum) return false;//6.2
        order o(in["-u"],in["-i"],DayID,sid,tid,required,1);
        //if (in.count("-debug")) std::cerr<<t.seat(sid,tid,DayID)<<std::endl;
        if (satisfied(t,o)){
            t.buy(sid,tid,DayID,required);
            pts->con.update(in["-i"],t);
            orderlist.insert(std::make_pair(std::make_pair(in["-u"],++ordernum),o));
            std::cout<<(unsigned long long)t.price(sid,tid)*o.num<<'\n';
            return true;
        }
        if (!in.count("-q")||in["-q"]=="false") return false;
        o.stat=0;
        orderlist.insert(std::make_pair(std::make_pair(in["-u"],++ordernum),o));
        pendingqueue.insert(std::make_pair(std::make_pair(exact_train(in["-i"],DayID),ordernum),o));
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
        train t=pts->con[o.trainID];
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
        pts->con.update(o.trainID,t);
        return true;
    }
    bool query_order(const parse &in){
        auto pr=orderlist.range_find(std::make_pair(in["-u"],0),std::make_pair(in["-u"],ordernum));
        std::cout<<pr.size()<<std::endl;
         for (auto i=(int)(pr.size())-1; i>=0; --i) {
            auto &tmp=pr[i].second;
            std::cout << "[" << stat[tmp.stat] << "] ";
            train t=pts->con[tmp.trainID];
            std::cout<<t.trainID<<" ";
            std::cout<<t.stations[tmp.sid]<<" ";
            std::cout<<t.leavingtime(tmp.sid,tmp.dayID)<<" ";
            std::cout<<"-> ";
            std::cout<<t.stations[tmp.tid]<<" ";
            std::cout<<t.arrivetime(tmp.tid,tmp.dayID)<<" ";
            std::cout<<t.price(tmp.sid,tmp.tid)<<" ";
            std::cout<<tmp.num<<'\n';
        }
        return true;
    }
};
#endif //TICKETSYSTEM_TICKETINNERSYSTEM_HPP
