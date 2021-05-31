//
// Created by Administrator on 2021/3/1.
//

#ifndef DEQUE_LIST_HPP
#define DEQUE_LIST_HPP

#include <cstdio>
#include <iostream>
namespace sjtu {
    template<class T>
    class list {
        struct node {
            T *value;
            node *ne;
            node *pre;

            node(const T &v) {
                value = new T(v);
                ne = nullptr;
                pre = nullptr;
            }
            ~node(){
//                std::cout<<"nodedelete"<<value<<" "<<std::endl;
                delete value;
            }
        };

        node *_head;
        node *_tail;
        size_t _size;
    public:
        class iterator{
            friend void link(iterator p,iterator q) {
                p.pos->ne=q.pos;
                q.pos->pre=p.pos;
            }
            friend void cut(iterator p,iterator q){
                p.pos->ne=nullptr;
                q.pos->pre=nullptr;
            }
            list<T> * rt;
            node *pos;
            size_t ind;
        public:
            list<T> * RT() const{
                return rt;
            }
            iterator():rt(nullptr),pos(nullptr),ind(0){
            }
            iterator(list<T> * const & rt,node * const & p,const size_t & i):rt(rt),pos(p),ind(i){
            }
            iterator & operator ++(){
                pos=pos->ne;
                ++ind;
                return *this;
            }
            const iterator operator ++(int){
                node *ret=pos;
                pos=pos->ne;
                return iterator(rt,ret,ind++);
            }
            iterator operator +(const size_t & u) const{
                node *tmp=pos;
                for (size_t i=0; i<u; ++i) tmp=tmp->ne;
                return iterator(rt,tmp,ind+u);
            }
            iterator operator -(const size_t &u) const{
                if (*this==rt->end())
                    return u?rt->tail()-(u-1):rt->end();
                node *tmp=pos;
                for (size_t i=0; i<u; ++i) tmp=tmp->pre;
                return iterator(rt,tmp,ind-u);
            }
            iterator& operator +=(const size_t &u){
                return *this=operator +(u);
            }
            iterator& operator -=(const size_t &u){
                return *this=operator -(u);
            }
            iterator operator --(){
                if (*this==rt->end()) return *this=rt->tail();
                pos=pos->pre;
                --ind;
                return *this;
            }
            operator node *(){
                return pos;
            }
            void destroy() const{
                delete pos;
            }
            const iterator operator --(int){
                if (*this==rt->end()){
                    *this=rt->tail();
                    return rt->end();
                }
                node *ret=pos;
                pos=pos->pre;
                return iterator(rt,ret,ind--);
            }
            int operator -(const iterator &it) const{
                return ind-it.ind;
            }
            T& operator *() const{
                return *(pos->value);
            }
            T * operator ->() const{
                return pos->value;
            }
            bool available() const{
                return pos!=nullptr;
            }
            bool operator ==(const iterator &it) const{
                return pos==it.pos&&ind==it.ind;
            }
            bool operator !=(const iterator &it) const{
                return pos!=it.pos||ind!=it.ind;
            }
            void print(){
                std::cout<<rt<<" "<<pos<<" "<<ind;
            }
        };
        list():_head(nullptr),_tail(nullptr),_size(0){
        }
        list(node * const &a,node * const &b,const size_t &c):_head(a),_tail(b),_size(c){
        }
        list(const list &other):_head(nullptr),_tail(nullptr),_size(0){
            for (auto i=other.begin(); i!=other.end(); ++i) push_back(*i);
        }
        list & operator=(const list &other){
            if (&other==this) return *this;
            clear();
            for (auto i=other.begin(); i!=other.end(); ++i) push_back(*i);
            return *this;
        }
        ~list(){
            for (iterator it=begin(); it!=end(); ) {
                (it++).destroy();
            }
        }
        void clear(){
            while (size()) pop_back();
        }
        void push_back(const T &v) {
            insert(end(),v);
        }

        void push_front(const T &v) {
            insert(begin(),v);
        }
        void pop_front(){
            erase(begin());
        }
        void pop_back(){
            erase(tail());
        }
        iterator begin() const{
            return iterator(const_cast<list<T > * > (this),_head, 0);
        }
        iterator head() const{
            return begin();
        }
        iterator tail() const {
            return iterator(const_cast<list<T > * > (this),_tail,size()-1);
        }
        iterator end() const {
            return iterator(const_cast<list<T > * > (this),nullptr,size());
        }
        bool empty() const{
            return _size==0;
        }
        size_t size() const{
            return _size;
        }
        iterator insert(const iterator & p,const T & v){
            iterator ret;
            if (empty()){
                _head=_tail=new node(v);
                ret=begin();
            }
            else if (p==end()){
                node *tmp=new node(v);
                link(tail(),ret=iterator(this,tmp,_size));//need to care and think
                _tail=tmp;
            }
            else if (p==begin()){
                node *tmp=new node(v);
                link(iterator(this,tmp,0),head());
                _head=tmp;
                ret=begin();
            }
            else{
                iterator tmp=p-1;
                iterator t=iterator(this,new node(v),p-begin());
                cut(tmp,p);
                link(tmp,t);
                link(t,p);
                ret=t;
            }
            ++_size;
            return ret;
        }
        iterator erase(const iterator &p){
            iterator ret;
            if (p==head()){
                if (p==tail()) _head=_tail=nullptr;
                else {
                    _head=(p+1);
                    cut(p, p + 1);
                }
                --_size;
                ret=begin();
            }
            else if (p==tail()){
                _tail=(p-1);
                cut(p-1,p);
                --_size;
                ret=end();
            }
            else{
                ret=iterator(this,p+1,p-begin());
                link(p-1,p+1);
                --_size;
            }
            p.destroy();
            return ret;
        }
        list * split(const size_t u){
            iterator p=begin()+u;

            list * ret=new list(p,tail(),size()-u);
            _tail=p-1;
            cut(p-1,p);
            _size=u;
            return ret;
        }
    };
}


#endif //DEQUE_LIST_HPP
