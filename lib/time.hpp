//
// Created by Administrator on 2021/5/12.
//
//modify
#ifndef TICKETSYSTEM_TIME_HPP
#define TICKETSYSTEM_TIME_HPP
#include <string>
#include <iostream>
#include <iomanip>
#include "algorithm.hpp"
typedef size_t UINT;
typedef std::string STR;
const UINT MIN_PER_HOUR=60u;
const UINT MIN_PER_DAY=24u*MIN_PER_HOUR;
const UINT DAY[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
struct Date;
struct ODate{
    UINT pass;//比1-1过了几天
    ODate(const UINT &u=0):pass(u){
    }
    ODate(const Date &);
    ODate operator +(const ODate &d) const{
        return pass+d.pass;
    }
    ODate operator -(const ODate &d) const{
        return pass-d.pass;
    }
    ODate& operator +=(const ODate &d){
        pass+=d.pass;
    }
    bool operator <(const ODate &d) const{
        return pass<d.pass;
    }
    bool operator >(const ODate &d) const{
        return pass>d.pass;
    }
    bool operator <=(const ODate &d) const{
        return pass<=d.pass;
    }
    bool operator >=(const ODate &d) const{
        return pass>=d.pass;
    }
    bool operator ==(const ODate &d) const{
        return pass==d.pass;
    }
    bool operator !=(const ODate &d) const{
        return pass!=d.pass;
    }
    operator UINT() const{
        return pass;
    }
};
struct Date{
    UINT month;
    UINT day;
    Date():month(1),day(1){
    }
    Date(const ODate &d){
        day=d.pass+1;
        month=1;
        while (day>DAY[month]) day-=DAY[month++];
    }
    Date(const STR &s){
        auto v=divide(s,'-');
        month=strtonum(v[0]);
        day=strtonum(v[1]);
    }
    Date operator +(const ODate &d) const{
        return Date(ODate(*this)+d);
    }
    ODate operator -(const Date &d) const{
        return ODate(*this)-ODate(d);
    }
    bool operator <(const Date &d) const{
        return ODate(*this)<ODate(d);
    }
    bool operator >(const Date &d) const{
        return ODate(*this)>ODate(d);
    }
    bool operator <=(const Date &d) const{
        return ODate(*this)<=ODate(d);
    }
    bool operator >=(const Date &d) const{
        return ODate(*this)>=ODate(d);
    }
    bool operator ==(const Date &d) const{
        return ODate(*this)==ODate(d);
    }
    bool operator !=(const Date &d) const{
        return ODate(*this)!=ODate(d);
    }
    friend std::ostream & operator <<(std::ostream &os,const Date &_){
        return os<<std::setw(2)<<std::setfill('0')<<_.month<<"-"<<std::setw(2)<<std::setfill('0')<<_.day;
    }
};
ODate::ODate(const Date &u){
    pass=u.day-1;
    for (UINT i=1; i<u.month; ++i) pass+=DAY[i];
}
struct Hour{
    UINT hour;
    Hour():hour(0){
    }
};
struct Minute{
    UINT minute;
    Minute():minute(0){
    }
};
struct Time;
struct OTime{
    UINT pass;
    OTime(UINT u=0):pass(u){
    }
    OTime(const Time &);
    OTime operator +(const OTime &_) const{
        return pass+_.pass;
    }
    OTime operator -(const OTime &_) const{
        return pass-_.pass;
    }
    OTime& operator +=(const OTime &_){
        pass+=_.pass;
    }
    bool operator <(const OTime &_) const{
        return pass<_.pass;
    }
    bool operator >(const OTime &_) const{
        return pass>_.pass;
    }
    bool operator <=(const OTime &_) const{
        return pass<=_.pass;
    }
    bool operator >=(const OTime &_) const{
        return pass>=_.pass;
    }
    bool operator ==(const OTime &_) const{
        return pass==_.pass;
    }
    bool operator !=(const OTime &_) const{
        return pass!=_.pass;
    }
    friend std::ostream & operator <<(std::ostream &os,const OTime &_);
};
struct Time{
    Date date;
    Hour hour;
    Minute minute;
    Time(OTime T=OTime()){//no yin yong
        date=ODate(T.pass/MIN_PER_DAY);
        T.pass%=MIN_PER_DAY;
        hour.hour=T.pass/MIN_PER_HOUR;
        minute.minute=T.pass%MIN_PER_HOUR;
    }
    Time operator +(const OTime &_) const{
        return Time(OTime(*this)+_);
    }
    OTime operator -(const Time &_) const{
        return OTime(*this)-OTime(_);
    }
    bool operator <(const Time &_) const{
        return OTime(*this)<OTime(_);
    }
    bool operator >(const OTime &_) const{
        return OTime(*this)>OTime(_);
    }
    bool operator <=(const Time &_) const{
        return OTime(*this)<=OTime(_);
    }
    bool operator >=(const OTime &_) const{
        return OTime(*this)>=OTime(_);
    }
    bool operator ==(const OTime &_) const{
        return OTime(*this)==OTime(_);
    }
    bool operator !=(const OTime &_) const{
        return OTime(*this)!=OTime(_);
    }
    friend std::ostream & operator <<(std::ostream &os,const Time &_){
        return os<<_.date<<" "<<std::setw(2)<<std::setfill('0')<<_.hour.hour<<":"<<std::setw(2)<<std::setfill('0')<<_.minute.minute;
    }
};
OTime::OTime(const Time &T){
    pass=ODate(T.date).pass*MIN_PER_DAY+T.hour.hour*MIN_PER_HOUR+T.minute.minute;
}
std::ostream & operator <<(std::ostream &os,const OTime &_){
    return os<<Time(_);
}
#endif //TICKETSYSTEM_TIME_HPP
