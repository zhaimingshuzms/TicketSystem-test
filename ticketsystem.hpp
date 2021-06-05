//
// Created by Administrator on 2021/4/30.
//

#ifndef TICKETSYSTEM_TICKETSYSTEM_HPP
#define TICKETSYSTEM_TICKETSYSTEM_HPP
#include "usersystem.hpp"
#include "trainsystem.hpp"
#include "ticketinnersystem.hpp"
#include <bits/stdc++.h>
long long sum=0;
class ticketsystem{
    usersystem us;
    trainsystem ts;
    ticketinnersystem tis;
    STR func_indtoname[16];
    sjtu::map<STR,UINT> func_nametoind;
    void (ticketsystem::*pp[16])(const parse &);
public:
    ticketsystem():tis(&ts){
        func_indtoname[0]="adduser";
        func_indtoname[1]="login";
        func_indtoname[2]="logout";
        func_indtoname[3]="query_profile";
        func_indtoname[4]="modify_profile";
        func_indtoname[5]="add_train";
        func_indtoname[6]="release_train";
        func_indtoname[7]="query_train";
        func_indtoname[8]="delete_train";
        func_indtoname[9]="query_ticket";
        func_indtoname[10]="query_transfer";
        func_indtoname[11]="buy_ticket";
        func_indtoname[12]="query_order";
        func_indtoname[13]="refund_ticket";
        func_indtoname[14]="clean";
        func_indtoname[15]="exit";
        pp[0]=&ticketsystem::adduser;
        pp[1]=&ticketsystem::login;
        pp[2]=&ticketsystem::logout;
        pp[3]=&ticketsystem::query_profile;
        pp[4]=&ticketsystem::modify_profile;
        pp[5]=&ticketsystem::add_train;
        pp[6]=&ticketsystem::release_train;
        pp[7]=&ticketsystem::query_train;
        pp[8]=&ticketsystem::delete_train;
        pp[9]=&ticketsystem::query_ticket;
        pp[10]=&ticketsystem::query_transfer;
        pp[11]=&ticketsystem::buy_ticket;
        pp[12]=&ticketsystem::query_order;
        pp[13]=&ticketsystem::refund_ticket;
        pp[14]=&ticketsystem::clean;
        pp[15]=&ticketsystem::exit;
        for (UINT i=0; i<16; ++i) func_nametoind[func_indtoname[i]]=i;
    }
    void adduser(const parse &in){
        std::cout<<us.add_user(in)-1<<'\n';
    }
    void login(const parse &in){
        std::cout<<us.login(in)-1<<'\n';
    }
    void logout(const parse &in){
        std::cout<<us.logout(in)-1<<'\n';
        //need to be done
    }
    void query_profile(const parse &in){
        //int t=clock();
        if (!us.query_profile(in)) std::cout<<-1<<'\n';
        //sum+=clock()-t;
        //need to be done
    }
    void modify_profile(const parse &in){
        if (!us.modify_profile(in)) std::cout<<-1<<'\n';
        //need to be done
    }
    void add_train(const parse &in){
        if (ts.add_train(in)) std::cout<<0<<'\n';
        else std::cout<<-1<<'\n';
        //std::cerr<<"Addend"<<std::endl;
        //need to be done
    }
    void release_train(const parse &in){
        //int t=clock();
        if (tis.release_train(in)) std::cout<<0<<'\n';
        else std::cout<<-1<<'\n';
        //sum+=clock()-t;
        //need to be done
    }
    void query_train(const parse &in){
        //int t=clock();
        if (!ts.query_train(in)) std::cout<<-1<<'\n';
        //sum+=clock()-t;
        //need to be done
    }
    void delete_train(const parse &in){
        //std::cerr<<"del:"<<in["-i"]<<std::endl;
        if (ts.delete_train(in)) std::cout<<0<<'\n';
        else std::cout<<-1<<'\n';
        //need to be done
    }
    void query_ticket(const parse &in){
        int t=clock();
        if (!tis.query_ticket(in)) std::cout<<-1<<'\n';
        sum+=t-clock();
        //need to be done
    }
    void query_transfer(const parse &in){
        //int t=clock();
        tis.query_transfer(in);
        //sum+=clock()-t;
        //need to be done
    }
    void buy_ticket(const parse &in){
        //need to be done
        //int t=clock();
        if (us.already_login(in["-u"])&&tis.buy_ticket(in));
        else std::cout<<-1<<'\n';
        //sum+=clock()-t;
    }
    void query_order(const parse &in){
        //int t=clock();
        if (us.already_login(in["-u"])&&tis.query_order(in));
        else std::cout<<-1<<std::endl;
        //sum+=clock()-t;
        //need to be done
    }
    void refund_ticket(const parse &in){
        if (us.already_login(in["-u"])&&tis.refund_ticket(in)) std::cout<<0<<'\n';
        else std::cout<<-1<<'\n';
        //need to be done
    }
    void clean(const parse &in){
        //need to be done
    }
    void exit(const parse &in){
        //need to be done
        std::cerr<<sum/1000.0<<std::endl;
        std::cout<<"bye"<<std::endl;
    }
public:
    bool respond(STR str){
        parse in(str);
        return judge(in);
    }
    bool judge(const parse &in){
        //if (in["-or"]=="buy_ticket"&&(in["-d"]=="07-02"||in["-d"]=="07-03")&&in["-i"]=="LeavesofGrass") std::cerr<<in["-u"]<<" "<<in["-f"]<<" "<<in["-t"]<<" "<<in["-d"]<<" "<<in["-n"]<<std::endl;
        (this->*pp[func_nametoind[in["-or"]]])(in);
        if (in["-or"]=="exit") return false;
        return true;
    }
};
#endif //TICKETSYSTEM_TICKETSYSTEM_HPP
