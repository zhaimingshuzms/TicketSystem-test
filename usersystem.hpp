//
// Created by Administrator on 2021/4/30.
//

#ifndef TICKETSYSTEM_USERSYSTEM_HPP
#define TICKETSYSTEM_USERSYSTEM_HPP
#include <string.h>
#include <utility>
#include <iostream>
#include "./lib/numas.hpp"
#include "./lib/mystring.hpp"
#include "lib/fakebpt.hpp"
#include "input_traits.hpp"
#include "BPlusTree.h"
typedef std::string STR;
typedef unsigned int UINT;
struct userdata{//er jin zhi wen jian xu yao
    MYSTR<21> username;
    MYSTR<31> password;
    MYSTR<21> name;
    MYSTR<31> mailAddr;
    UINT privilege;
    userdata(const MYSTR<21> &s1=MYSTR<21>(""),const MYSTR<31> &s2=MYSTR<31>(""),const MYSTR<21> &s3=MYSTR<21>(""),const MYSTR<31> &s4=MYSTR<31>(""),const UINT &u=0):
            username(s1),password(s2),name(s3),mailAddr(s4),privilege(u){
    }
    friend std::ostream & operator <<(std::ostream & os,const userdata &ud){
        return os<<ud.username<<" "<<ud.name<<" "<<ud.mailAddr<<" "<<ud.privilege;
    }
};
class usersystem{
    BPlusTree<MYSTR<21>,userdata> con;
    sjtu::map<MYSTR<21>,bool> loginlist;
public:
    usersystem():con("user.bin"){
    }
    bool already_login(const MYSTR<21> &s){
        if (loginlist.count(s)) return true;
        return false;
    }
    bool add_user(const parse & in){
        if (con.empty()){
            con.insert(std::make_pair(
                    in["-u"],userdata(in["-u"],in["-p"],in["-n"],in["-m"],10)));
        }
        else{
            //chong fu key
            if (!loginlist.count(in["-c"])||con[in["-c"]].privilege<=strtonum(in["-g"])||con.count(in["-u"])) return false;
            con.insert(std::make_pair(
                    in["-u"],userdata(in["-u"],in["-p"],in["-n"],in["-m"],strtonum(in["-g"]))));
        }
        return true;
    }
    bool login(const parse & in){
        if (!loginlist.count(in["-u"])&&con.count(in["-u"])&&con[in["-u"]].password==in["-p"]){
            loginlist[in["-u"]]=true;
            return true;
        }
        return false;
    }
    bool logout(const parse & in){
        if (loginlist.count(in["-u"])){
            loginlist.erase(loginlist.find(in["-u"]));
            return true;
        }
        return false;
    }
    bool query_profile(const parse & in){
        if (loginlist.count(in["-c"])&&con.count(in["-u"])&&(con[in["-c"]].privilege>con[in["-u"]].privilege||in["-c"]==in["-u"])){
            std::cout<<con[in["-u"]]<<'\n';
            return true;
        }
        return false;
    }
    bool modify_profile(const parse &in){
        if (loginlist.count(in["-c"])&&con.count(in["-u"])&&(con[in["-c"]].privilege>con[in["-u"]].privilege||in["-c"]==in["-u"])){
            auto tmp=con[in["-u"]];
            if (in.count("-g")) {
                if (strtonum(in["-g"]) < con[in["-c"]].privilege)
                    tmp.privilege = strtonum(in["-g"]);
                else return false;
            }
            if (in.count("-p")) tmp.password=in["-p"];
            if (in.count("-n")) tmp.name=in["-n"];
            if (in.count("-m")) tmp.mailAddr=in["-m"];
            con.update(in["-u"],tmp);
            std::cout<<con[in["-u"]]<<'\n';
            return true;
        }
        return false;
    }
};
#endif //TICKETSYSTEM_USERSYSTEM_HPP