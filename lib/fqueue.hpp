//
// Created by Administrator on 2021/5/19.
//

#ifndef TICKETSYSTEM_FQUEUE_HPP
#define TICKETSYSTEM_FQUEUE_HPP
#include <string>
template<class T>
class fqueue{
    fqueue(const std::string &);
    size_t size() const;
    bool empty() const;
    T front() const;
    T push();
    void pop();
};
#endif //TICKETSYSTEM_FQUEUE_HPP
