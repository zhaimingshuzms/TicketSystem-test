#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
    std::allocator<T> alloc;
    T * head;
    size_t _size,_cap;
public:
    void recap(){
        std::allocator<T> b;
        T * tmp=b.allocate(_cap*2);
        for (size_t i=0; i<_cap; ++i) b.construct(tmp+i,head[i]);
        for (size_t i=0; i<_cap; ++i) alloc.destroy(head+i);
        alloc.deallocate(head,_cap);
        _cap*=2;
        alloc=b;
        head=tmp;
    }
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		T *pos;
		const vector* orz;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator(T * const u,const vector* org):pos(u),orz(org){
		}
		bool operator <(const iterator & p) const{
		    return pos<p.pos;
		}
		iterator operator+(const int &n) const {
			//TODO
			return iterator(pos+n,orz);
		}
		iterator operator-(const int &n) const {
			//TODO
			return iterator(pos-n,orz);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			//TODO
			return pos-rhs.pos;
		}
		iterator& operator+=(const int &n) {
			//TODO
			pos+=n;
			return *this;
		}
		iterator& operator-=(const int &n) {
			//TODO
			pos-=n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
            iterator tmp=*this;
            ++pos;
            return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
            ++pos;
            return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            iterator tmp=*this;
            --pos;
            return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
            --pos;
            return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
		    return *pos;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
		    return pos==rhs.pos&&orz==rhs.orz;
		}
		bool operator==(const const_iterator &rhs) const {
            return pos==rhs.pos&&orz==rhs.orz;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
            return pos!=rhs.pos||orz!=rhs.orz;
		}
		bool operator!=(const const_iterator &rhs) const {
            return pos!=rhs.pos||orz!=rhs.orz;
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
        T *pos;
        const vector* orz;
    public:
        const_iterator(T * const u,const vector* org):pos(u),orz(org){
        }
        bool operator <(const const_iterator & p) const{
            return pos<p.pos;
        }
        const_iterator operator+(const int &n) const {
            //TODO
            return const_iterator(pos+n,orz);
        }
        const_iterator operator-(const int &n) const {
            //TODO
            return const_iterator(pos-n,orz);
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            //TODO
            return pos-rhs.pos;
        }
        const_iterator& operator+=(const int &n) {
            //TODO
            pos+=n;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            //TODO
            pos-=n;
            return *this;
        }
        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            const_iterator tmp=*this;
            ++pos;
            return tmp;
        }
        /**
         * TODO ++iter
         */
        const_iterator& operator++() {
            ++pos;
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            const_iterator tmp=*this;
            --pos;
            return tmp;
        }
        /**
         * TODO --iter
         */
        const_iterator& operator--() {
            --pos;
            return *this;
        }
        /**
         * TODO *it
         */
        const T& operator*() const{
            return *pos;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            return pos==rhs.pos&&orz==rhs.orz;
        }
        bool operator==(const const_iterator &rhs) const {
            return pos==rhs.pos&&orz==rhs.orz;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return pos!=rhs.pos||orz!=rhs.orz;
        }
        bool operator!=(const const_iterator &rhs) const {
            return pos!=rhs.pos||orz!=rhs.orz;
        }
	};
	/**
	 * TODO Constructs
	 * Atleast two: default constructor, copy constructor
	 */
	vector() {
	    _size=0;
	    _cap=2000;
	    head=alloc.allocate(_cap);
	}
	vector(const vector &other) {
	    _size=other._size;
	    _cap=other._cap;
	    head=alloc.allocate(_cap);
	    for (size_t i=0; i<_size; ++i) alloc.construct(head+i,other.head[i]);
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
	    for (size_t i=0; i<_size; ++i) alloc.destroy(head+i);
	    alloc.deallocate(head,_cap);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
	    if (this==&other) return *this;
	    for (size_t i=0; i<_size; ++i) alloc.destroy(head+i);
	    alloc.deallocate(head,_cap);

        _size=other._size;
        _cap=other._cap;
        head=alloc.allocate(_cap);
        for (size_t i=0; i<_size; ++i) alloc.construct(head+i,other.head[i]);
	    return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
        if (pos>=_size||pos<0) throw index_out_of_bound();
	    return head[pos];
	}
	const T & at(const size_t &pos) const {
        if (pos>=_size||pos<0) throw index_out_of_bound();
	    return head[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
	    if (pos>=_size||pos<0) throw index_out_of_bound();
	    return head[pos];
	}
	const T & operator[](const size_t &pos) const {
        if (pos>=_size||pos<0) throw index_out_of_bound();
	    return head[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
	    if (empty()) throw container_is_empty();
	    return head[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
        if (empty()) throw container_is_empty();
	    return head[_size-1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
	    return iterator(head,this);
	}
	const_iterator cbegin() const {
	    return const_iterator(head,this);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
	    return iterator(head+_size,this);
	}
	const_iterator cend() const {
	    return const_iterator(head+_size,this);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
	    return _size==0;
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
	    for (size_t i=0; i<_size; ++i) alloc.destroy(head+i);
	    _size=0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
	    return insert(pos-begin(),value);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
	    if (ind>_size) throw index_out_of_bound();
        if (_size==_cap) recap();
        for (int i=_size; i>ind; --i) head[i]=head[i-1];
        alloc.construct(head+ind,value);
        ++_size;
        return iterator(head+ind,this);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
	    return erase(pos-begin());
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
        if (ind>=_size) throw index_out_of_bound();
        alloc.destroy(head+ind);
        for (int i=ind; i+1<_size; ++i) head[i]=head[i+1];
        --_size;
        return iterator(head+ind,this);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
	    if (_size==_cap) recap();
        alloc.construct(head+(_size++),value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
	    if (empty()) throw container_is_empty();
	    alloc.destroy(head+(_size--));
	}
};


}

#endif
