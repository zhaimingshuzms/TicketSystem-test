/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	struct node{
	    value_type *val;
        bool color;//0 red 1 black
	    node *p,*left,*right;
	    node():val(nullptr),color(1),p(this),left(this),right(this){
	    }
	    //node(const value_type &);
        node(const value_type &u,node * const &NIL):color(0),p(NIL->p),left(NIL->left),right(NIL->right){
            val=new value_type(u);
	    }
	    ~node(){
//	        std::cerr<<"wtf"<<std::endl;
	        if (val!=nullptr) delete val;
	    }
	};
    node *nil;

    node * root;

    size_t _size;
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	private:
	    map * rt;
	    node * p;
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
	    map * const & RT() const{
	        return rt;
	    }
	    node * const & P() const{
	        return p;
	    }
		iterator():rt(nullptr),p(nullptr) {
			// TODO
		}
		iterator(map * const & rt,node * const & p):rt(rt),p(p){

		}
		iterator(const iterator &other):rt(other.rt),p(other.p) {
			// TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    iterator ret=*this;
		    operator ++();
		    return ret;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
		    if (*this==rt->end()) throw invalid_iterator();
		    p=rt->NEXT(p);
		    return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            iterator ret=*this;
            operator --();
            return ret;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
		    if (*this==rt->begin()) throw invalid_iterator();
            if (*this==rt->end()) p=rt->TREE_MAXIMUM(rt->root);
            else p=rt->PREV(p);
            return *this;
		}
		/**
		 * an operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
		    return *(p->val);
		}
		bool operator==(const iterator &rhs) const {
		    return rt==rhs.rt&&p==rhs.p;
		}
		bool operator==(const const_iterator &rhs) const {
            return rt==rhs.RT()&&p==rhs.P();
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
		    return rt!=rhs.rt||p!=rhs.p;
		}
		bool operator!=(const const_iterator &rhs) const {
            return rt!=rhs.RT()||p!=rhs.P();
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
		    return p->val;
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
            const map * rt;
            const node * p;
		public:
            const map * const & RT() const{
                return rt;
            }
            const node * const & P() const{
                return p;
            }
            const_iterator(const map * const & rt,const node * const & p):rt(rt),p(p){

            }
			const_iterator():rt(nullptr),p(nullptr){
				// TODO
			}
			const_iterator(const const_iterator &other):rt(other.rt),p(other.p){
				// TODO
			}
			const_iterator(const iterator &other):rt(other.RT()),p(other.P()){
				// TODO
			}
        /**
     * TODO iter++
     */
        const_iterator operator++(int) {
            const_iterator ret=*this;
            operator ++();
            return ret;
        }
        /**
         * TODO ++iter
         */
        const_iterator & operator++() {
            if (*this==rt->end()) throw invalid_iterator();
            p=rt->NEXT(const_cast<node *>(p));
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            const_iterator ret=*this;
            operator --();
            return ret;
        }
        /**
         * TODO --iter
         */
        const_iterator & operator--() {
            if (*this==rt->begin()) throw invalid_iterator();
            if (*this==rt->cend()) p=rt->TREE_MAXIMUM(rt->root);
            else p=rt->PREV(const_cast<node *>(p));
            return *this;
        }
        /**
         * an operator to check whether two iterators are same (pointing to the same memory).
         */
        const value_type & operator*() const {
            return *(p->val);
        }
        bool operator==(const iterator &rhs) const {
            return rt==rhs.RT()&&p==rhs.P();
        }
        bool operator==(const const_iterator &rhs) const {
            return rt==rhs.rt&&p==rhs.p;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return rt!=rhs.RT()||p!=rhs.P();
        }
        bool operator!=(const const_iterator &rhs) const {
            return rt!=rhs.rt||p!=rhs.p;
        }

        /**
         * for the support of it->first.
         * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
         */
        const value_type* operator->() const noexcept {
            return p->val;
        }
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
	};
	/**
	 * TODO two constructors
	 */
	map():nil(new node()),root(nil),_size(0){}
	map(const map &other):nil(new node()),root(nil),_size(0){
	    //slow;
        for (auto i=other.begin(); i!=other.end(); ++i) insert(*i);
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
	    if (this==&other) return *this;
	    //slow;
	    clear();
	    for (auto i=other.begin(); i!=other.end(); ++i) insert(*i);
	    return *this;
	}
	/**
	 * TODO Destructors
	 */
	 void destroy(node *x){
	     if (x==nil) return;
	     destroy(x->left);
	     destroy(x->right);
	     delete x;
	 }
	~map() {
	    destroy(root);
        delete nil;
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
        auto it=find(key);
        if (it==end()) throw index_out_of_bound();
        return it->second;
	}
	const T & at(const Key &key) const {
        auto it=find(key);
        if (it==end()) throw index_out_of_bound();
        return it->second;
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
	    return insert(pair<Key,T>(key,T())).first->second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
        return at(key);
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() const{
	    return iterator(const_cast<map *>(this),TREE_MINIMUM(root));
	}
	const_iterator cbegin() const {
        return const_iterator(const_cast<map *>(this),TREE_MINIMUM(root));
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() const{
	    return iterator(const_cast<map *>(this),nil);
	}
	const_iterator cend() const {
        return const_iterator(const_cast<map *>(this),nil);
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
	    return size()==0;
	}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
	    return _size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
	    //???need to worry
	    map * p=new map();
	    std::swap(root,p->root);
	    std::swap(nil,p->nil);
	    std::swap(_size,p->_size);
	    delete p;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	void LEFT_ROTATE(node *x){
	    node * y=x->right;
	    x->right=y->left;
	    if (y->left!=nil) y->left->p=x;
	    y->p=x->p;
	    if (x->p==nil) root=y;
	    else if (x->p->left==x) x->p->left=y;
	    else x->p->right=y;
	    y->left=x;
	    x->p=y;
	}
	void RIGHT_ROTATE(node *x){
        node * y=x->left;
        x->left=y->right;
        if (y->right!=nil) y->right->p=x;
        y->p=x->p;
        if (x->p==nil) root=y;
        else if (x->p->right==x) x->p->right=y;
        else x->p->left=y;
        y->right=x;
        x->p=y;
	}
	void RB_INSERT_FIXUP(node * z){
        while (z->p->color==0){
            if (z->p==z->p->p->left){
                node * y=z->p->p->right;
                if (y->color==0){
                    z->p->color=y->color=1;
                    y->p->color=0;
                    z=y->p;
                }
                else{
                    if (z->p->right==z){
                        z=z->p;
                        LEFT_ROTATE(z);
                    }
                    z->p->color=1;
                    z->p->p->color=0;
                    RIGHT_ROTATE(z->p->p);
                }
            }
            else{
                node * y=z->p->p->left;
                if (y->color==0){
                    z->p->color=y->color=1;
                    y->p->color=0;
                    z=y->p;
                }
                else{
                    if (z->p->left==z){
                        z=z->p;
                        RIGHT_ROTATE(z);
                    }
                    z->p->color=1;
                    z->p->p->color=0;
                    LEFT_ROTATE(z->p->p);
                }
            }
        }
        root->color=1;
	}
	iterator RB_INSERT(node &z){
        ++_size;
	    node *y=nil;
        node *x=root;
        while (x!=nil){
            y=x;
            //?????
            if (Compare()(z.val->first,x->val->first)){
                x=x->left;
            }
            else x=x->right;
        }
        z.p=y;
        if (y==nil) root=&z;
        else if (Compare()(z.val->first,y->val->first)) y->left=&z;
        else y->right=&z;
        //????
        RB_INSERT_FIXUP(&z);
        return iterator(const_cast<map *>(this),&z);
	}
	sjtu::pair<iterator, bool> insert(const value_type &value) {
	    iterator it=find(value.first);
	    if (it!=end()) return {it,false};
	    node * tmp=new node(value,nil);
	    return {RB_INSERT(*tmp),true};
	}
	void print(node *x) const{
	    if (x==nil) return;
	    print(x->left);
	    std::cout<<(x->val->first)<<" "<<(x->val->second)<<std::endl;
	    print(x->right);
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void RB_TRANSPLANT(node *u,node *v){
	    if (u->p==nil) root=v;
	    else if (u==u->p->left) u->p->left=v;
	    else u->p->right=v;
	    v->p=u->p;
	}
	void RB_DELETE_FIXUP(node *x){
	    while (x!=root&&x->color==1){
	        if (x==x->p->left){
	            node * w=x->p->right;
	            //case 1 turns
	            if (w->color==0){
	                w->color=1;
	                x->p->color=0;
	                LEFT_ROTATE(x->p);
	                w=x->p->right;
	            }
                //not else if
                if (w->left->color==1&&w->right->color==1){
                    w->color=0;
                    x=x->p;
                }
                else{
                    if (w->right->color==1) {
                        w->color=0;
                        w->left->color=1;
                        RIGHT_ROTATE(w);
                        w=x->p->right;
                    }
                    w->color=x->p->color;
                    x->p->color=1;
                    w->right->color=1;
                    LEFT_ROTATE(x->p);
                    break;
                }
	        }
	        else{
                node * w=x->p->left;
                //case 1 turns
                if (w->color==0){
                    w->color=1;
                    x->p->color=0;
                    RIGHT_ROTATE(x->p);
                    w=x->p->left;
                }
                //not else if
                if (w->right->color==1&&w->left->color==1){
                    w->color=0;
                    x=x->p;
                }
                else{
                    if (w->left->color==1) {
                        w->color=0;
                        w->right->color=1;
                        LEFT_ROTATE(w);
                        w=x->p->left;
                    }
                    w->color=x->p->color;
                    x->p->color=1;
                    w->left->color=1;
                    RIGHT_ROTATE(x->p);
                    break;
                }
            }
	    }
	    x->color=1;
	}
	node * TREE_MINIMUM(node *x) const{
	    while (x->left!=nil) x=x->left;
	    return x;
	}
    node * TREE_MAXIMUM(node *x) const{
        while (x->right!=nil) x=x->right;
        return x;
    }
	node * NEXT(node *x) const{
	    if (x->right!=nil) return TREE_MINIMUM(x->right);
	    while (x==x->p->right) x=x->p;
	    return x->p;
	}
	node *PREV(node *x) const{
        if (x->left!=nil) return TREE_MAXIMUM(x->left);
        while (x==x->p->left) x=x->p;
        return x->p;
	}
	void RB_DELETE(node *z){
	    --_size;
        node *y=z,*x;
        bool y_original_color=z->color;
	    if (z->left==nil){
	        x=z->right;
            RB_TRANSPLANT(z,z->right);
	    }
	    else if (z->right==nil){
	        x=z->left;
	        RB_TRANSPLANT(z,z->left);
	    }
	    else{
	        y=TREE_MINIMUM(z->right);
	        y_original_color=y->color;
	        x=y->right;
	        //change
	        if (y->p==z) x->p=y;
	        else{
                RB_TRANSPLANT(y,y->right);
                y->right=z->right;
                y->right->p=y;
	        }
	        RB_TRANSPLANT(z,y);
	        y->left=z->left;
	        y->left->p=y;
	        y->color=z->color;
	    }
	    if (y_original_color==1) RB_DELETE_FIXUP(x);
	}
	void erase(iterator pos) {
        //care of memory leak
	    if (pos.RT()!=this||pos==end()) throw invalid_iterator();
	    RB_DELETE(pos.P());
	    delete pos.P();
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
	    return find(key)!=end();
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	bool Equal(const Key &a,const Key &b) const{
	    return !(Compare()(a,b)||Compare()(b,a));
	}
	node * RB_FIND(node *x,const Key &key) const{
	    if (x==nil) return nil;
	    if (Equal(x->val->first,key)) return x;
	    if (Compare()(key,x->val->first)) return RB_FIND(x->left,key);
	    return RB_FIND(x->right,key);
	}
	iterator find(const Key &key) {
	    return iterator(const_cast<map *>(this),RB_FIND(root,key));
	}
	const_iterator find(const Key &key) const {
        return const_iterator(const_cast<map *>(this),RB_FIND(root,key));
	}
};
    /*template<
            class Key,
            class T,
            class Compare
    >
    map<Key,T,Compare>::node::node(const value_type &u):color(0),p(&(map<Key,T,Compare>::nil)),left(&map::nil),right(&map::nil){
        key=new Key(u.first);
        val=new T(u.second);
    }*/
}

#endif
