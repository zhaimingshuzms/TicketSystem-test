#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include <cstddef>
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
template<class T>
class deque {
    static const int BLOCK_SIZE=300;
    list<list<T> > l;
    size_t _size;
public:
	class const_iterator;
	class iterator {
	private:
        typename list<list<T> >::iterator p;
        typename list<T>::iterator q;
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   if there are not enough elements, iterator becomes invalid
		 * as well as operator-
		 */
        typename list<list<T> >::iterator & P(){
            return p;
        }
        typename list<T>::iterator & Q(){
            return q;
        }
        typename list<list<T> >::iterator const & P() const{
            return p;
        }
        typename list<T>::iterator const & Q() const{
            return q;
        }
        iterator(){
        }
		iterator(typename list<list<T> >::iterator const & pp,typename list<T>::iterator const & qq):p(pp),q(qq){
		}
		iterator operator+(const int &n) const {
            if (n<0) return operator -(-n);
		    int shift=n;
		    iterator tmp=*this;
		    //assert n=0
		    if (!tmp.p.available()) return tmp;
		    while (tmp.q-(tmp.p->begin())+shift>=tmp.p->size()){
		        shift-=tmp.p->size()-(tmp.q-(tmp.p->begin()));
		        ++tmp.p;
		        if (tmp.p.available()) tmp.q=tmp.p->head();
		        else{
		            tmp.q=(--tmp.p)->end();
		            return tmp;
		        }
		    }
		    tmp.q+=shift;
		    return tmp;
			//TODO
		}
		iterator operator-(const int &n) const {
			//TODO
            //todotest
            //slow
            if (n<0) return operator +(-n);
            int shift=n;
            iterator tmp=*this;
            //assert n=0
            if (!tmp.p.available()) return tmp;
            while (tmp.q-(tmp.p->begin())-shift<0){
			    shift-=tmp.q-(tmp.p->begin());
			    tmp.q=(--tmp.p)->end();
			}
			tmp.q-=shift;
			return tmp;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		size_t calc() const{
            typename list<list<T> >::iterator tmp1=p;
            typename list<T>::iterator tmp2=q;
            size_t ret=0;
            while (1){
                if (!(--tmp2).available()) break;
                ++ret;
            }
            while (1){
                if (!(--tmp1).available()) break;
                ret+=tmp1->size();
            }
            return ret;
        }
		int operator-(const iterator &rhs) const {
            //TODO
            if (p.RT()!=rhs.p.RT()) throw invalid_iterator();
            return calc()-rhs.calc();
		}
		iterator& operator+=(const int &n) {
		    return *this=operator +(n);
			//TODO
		}
		iterator& operator-=(const int &n) {
			//TODO
			return *this=operator -(n);
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
            if (!q.available()) throw invalid_iterator();
		    //lazy
		    iterator ret=*this;
		    operator ++();
		    return ret;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
		    if (!q.available()) throw invalid_iterator();
		    if (p->end()==q+1){
		        if (!(p+1).available()) ++q;
		        else q = (++p)->begin();
		    }
		    else ++q;
		    return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            if (!(p-1).available()&&q==p->begin()) throw invalid_iterator();
            iterator ret=*this;
            operator --();
            return ret;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
            if (!(p-1).available()&&q==p->begin()) throw invalid_iterator();
		    if (p->begin()==q) q=(--p)->tail();
		    else --q;
		    return *this;
		}
		/**
		 * TODO *it
		 * 		throw if iterator is invalid
		 */
		T& operator*() const {
		    if (q.available()) return *q;
		    else throw invalid_iterator();
		}
		/**
		 * TODO it->field
		 * 		throw if iterator is invalid
		 */
		T* operator->() const noexcept {
		    return q.operator ->();
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
		    return p==rhs.p&&q==rhs.q;
		}
		bool operator==(const const_iterator &rhs) const {
            return p==rhs.P()&&q==rhs.Q();
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
		    return p!=rhs.p||q!=rhs.q;
		}
		bool operator!=(const const_iterator &rhs) const {
            return p!=rhs.P()||q!=rhs.Q();
		}
		void print(){
		    std::cout<<"(";
		    p.print();
		    std::cout<<",";
		    q.print();
		    std::cout<<')'<<std::endl;
		}
	};
	class const_iterator {
    private:
        typename list<list<T> >::iterator p;
        typename list<T>::iterator q;
        /**
         * TODO add data members
         *   just add whatever you want.
         */
    public:
        /**
         * return a new iterator which pointer n-next elements
         *   if there are not enough elements, iterator becomes invalid
         * as well as operator-
         */
        typename list<list<T> >::iterator & P(){
            return p;
        }
        typename list<T>::iterator & Q(){
            return q;
        }
        typename list<list<T> >::iterator const & P() const{
            return p;
        }
        typename list<T>::iterator const & Q() const{
            return q;
        }
        const_iterator operator+(const int &n) const {
            if (n<0) return operator -(-n);
            int shift=n;
            const_iterator tmp=*this;
            //assert n=0
            if (!tmp.p.available()) return tmp;
            while (tmp.q-(tmp.p->begin())+shift>=tmp.p->size()){
                shift-=tmp.p->size()-(tmp.q-(tmp.p->begin()));
                ++tmp.p;
                if (tmp.p.available()) tmp.q=tmp.p->head();
                else{
                    tmp.q=(--tmp.p)->end();
                    return tmp;
                }
            }
            tmp.q+=shift;
            return tmp;
            //TODO
        }
        const_iterator operator-(const int &n) const {
            //TODO
            //todotest
            //slow
            if (n<0) return operator +(-n);
            int shift=n;
            const_iterator tmp=*this;
            //assert n=0
            if (!tmp.p.available()) return tmp;
            while (tmp.q-(tmp.p->begin())-shift<0){
                shift-=tmp.q-(tmp.p->begin());
                tmp.q=(--tmp.p)->end();
            }
            tmp.q-=shift;
            return tmp;
        }
        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        size_t calc() const{
            typename list<list<T> >::iterator tmp1=p;
            typename list<T>::iterator tmp2=q;
            size_t ret=0;
            while (1){
                if (!(--tmp2).available()) break;
                ++ret;
            }
            while (1){
                if (!(--tmp1).available()) break;
                ret+=tmp1->size();
            }
            return ret;
        }
        int operator-(const const_iterator &rhs) const {
            //TODO
            return calc()-rhs.calc();
        }
        const_iterator& operator+=(const int &n) {
            return *this=operator +(n);
            //TODO
        }
        const_iterator& operator-=(const int &n) {
            //TODO
            return *this=operator -(n);
        }
        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            //lazy
            if (!q.available()) throw invalid_iterator();
            const_iterator ret=*this;
            operator ++();
            return ret;
        }
        /**
         * TODO ++iter
         */
        const_iterator& operator++() {
            if (!q.available()) throw invalid_iterator();
            if (p->end()==q+1){
                if (!(p+1).available()) ++q;
                else q = (++p)->begin();
            }
            else ++q;
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            if (!(p-1).available()&&q==p->begin()) throw invalid_iterator();
            const_iterator ret=*this;
            operator --();
            return ret;
        }
        /**
         * TODO --iter
         */
        const_iterator& operator--() {
            if (!(p-1).available()&&q==p->begin()) throw invalid_iterator();
            if (p->begin()==q) q=(--p)->tail();
            else --q;
            return *this;
        }
        /**
         * TODO *it
         * 		throw if iterator is invalid
         */
        const T& operator*() const {
            return *q;
        }
        /**
         * TODO it->field
         * 		throw if iterator is invalid
         */
        const T* operator->() const noexcept {
            return q.operator ->();
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return p==rhs.p&&q==rhs.q;
        }
        bool operator==(const const_iterator &rhs) const {
            return p==rhs.P()&&q==rhs.Q();
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return p!=rhs.P()||q!=rhs.Q();
        }
        bool operator!=(const const_iterator &rhs) const {
            return p!=rhs.p||q!=rhs.q;
        }
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		const_iterator() {
            // TODO
        }
        const_iterator(const const_iterator &other):p(other.p),q(other.q){
            // TODO
        }
        const_iterator(const iterator &other) :p(other.P()),q(other.Q()){

            // TODO
        }
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
	};
	/**
	 * TODO Constructors
	 */
	deque():_size(0) {}
	deque(const deque &other):l(other.l),_size(other._size) {
	}
	/**
	 * TODO Deconstructor
	 */
	~deque() {}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {
	    if (&other==this) return *this;
	    l=other.l;
	    _size=other._size;
	    return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
	    if (pos<_size) return *(begin()+pos);
        throw index_out_of_bound();
	}
	const T & at(const size_t &pos) const {
        if (pos<_size)  return *(begin()+pos);
        throw index_out_of_bound();
	}
	T & operator[](const size_t &pos) {
        if (pos<_size)  return *(begin()+pos);
        throw index_out_of_bound();
	}
	const T & operator[](const size_t &pos) const {
        if (pos<_size)  return *(begin()+pos);
        throw index_out_of_bound();
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
	    if (empty()) throw container_is_empty();
	    return *begin();
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
        if (empty()) throw container_is_empty();
	    return *(end()-1);
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() const {
	    if (empty()) return iterator(typename list<list<T> >::iterator(const_cast<list<list<T> > *>(&l),nullptr,0),typename list<T>::iterator(nullptr,nullptr,0));
	    return iterator(l.head(),l.head()->head());
	}
	const_iterator cbegin() const {
	    return const_iterator(begin());
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() const {
	    if (empty()) return iterator(typename list<list<T> >::iterator(const_cast<list<list<T> > *>(&l),nullptr,0),typename list<T>::iterator(nullptr,nullptr,0));
	    return iterator(l.tail(),l.tail()->end());
	}
	const_iterator cend() const {
	    return const_iterator(end());
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {
	    return size()==0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
	    return _size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
	    l.clear();
	    _size=0;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
	    //empty
	    if (pos.P().RT()!=&l) throw invalid_iterator();
	    if (empty()){
	        list<T> *ret=new list<T>();
	        ret->push_back(value);
	        l.push_back(*ret);
	        delete ret;
	        ++_size;
	        return begin();
	    }
	    if (pos.P()->size()>=2*BLOCK_SIZE){
	        list<T> * tmp=pos.P()->split(BLOCK_SIZE);
            size_t shift=pos.Q()-(pos.P()->begin());
            l.insert(pos.P()+1,*tmp);
            delete tmp;
            if (shift>=BLOCK_SIZE){
	            shift-=BLOCK_SIZE;
	            pos.P()++;
	            pos.Q()=pos.P()->begin()+shift;
	        }
	    }
	    ++_size;
	    return iterator(pos.P(),pos.P()->insert(pos.Q(),value));
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
        if (pos.P().RT()!=&l||pos==end()) throw invalid_iterator();
	    --_size;//care
	    iterator ret=iterator(pos.P(),pos.P()->erase(pos.Q()));
	    if (pos.P()->empty()){
	        size_t shift=pos.P()-l.begin();
	        typename list<list<T> >::iterator tmp=l.erase(pos.P());
	        if (tmp==l.end()) ret=end();
	        else{
	            tmp=l.begin()+shift;
	            ret=iterator(tmp,tmp->begin());
	        }
	    }
	    else if (ret.Q()==ret.P()->end()&&ret!=end())
	        ret.Q()=(++ret.P())->begin();
	    return ret;
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
	    insert(end(),value);
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
	    if (empty()) throw container_is_empty();
	    erase(end()-1);
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
	    insert(begin(),value);
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
        if (empty()) throw container_is_empty();
	    erase(begin());
	}
};

}

#endif
