#ifndef TINYSTL_VECTOR_H
#define TINYSLT_VECTOR_H

#include <initializer_list>
#include <algorithm>
#include "alloc.h"

namespace lanstl {

// vector 模板类
template <class T>
class vector {
public:
    typedef lanstl::allocator<T>        allocator_type;
    typedef lanstl::allocator<T>        data_allocator;

    typedef typename allocator_type::value_type  value_type;
    typedef typename allocator_type::pointer     pointer;
    typedef typename allocator_type::size_type   size_type;
    typedef typename allocator_type::difference_type   difference_type;
    typedef typename allocator_type::reference         reference;

    typedef value_type*                 iterator;

    allocator_type get_allocator() { return data_allocator(); }

private:
    iterator        begin_;
    iterator        end_;
    iterator        cap_;     
    allocator_type  allocator_; 
    size_type       len_;        

public:
    //construct
    vector(size_type n,const value_type& value) {
        begin_ = allocator_.allocate(2*n);
        end_ = begin_ + n;
        cap_ = end_ + n;
        fill_init(n,value);
    }
    //deconstruct
    ~vector() {
        destroy_v(begin_,end_,cap_- begin_);
        begin_ = end_ = cap_ = nullptr;
    }

    void push_back(const value_type& val);
    void pop_back();
    // overload operator[]
    reference operator[](size_type n) {
        // TODO exception
        return *(begin_+ n);
    }

    size_type size() const {
        return static_cast<size_type>(len_);
    }

private:
    // some helper functions
    void fill_init(size_type n,const value_type& value);
    void destroy_v(pointer first,pointer last,size_type n);

};

// fill_init
template <class T>
void vector<T>:: fill_init(size_type n, const value_type& value) {
    len_ = n;
    fill_n(begin_,n,value);
}

template <class iter,class size,class T>
iter fill_n(iter first,size n,const T& val) {
    //len_ = 0;
    for(;n>0;--n,++first) {
        *first = val;
      //  len_++;
    }
    return first;
}

// destroy_v
template <class T>
void vector<T>:: destroy_v(pointer first,pointer last,size_type n) {
    allocator_.destroy(first,last);
    allocator_.deallocate(first,n);
}

// push_back
template <class T> 
void vector<T>::push_back(const value_type& value) {
    // std::cout << value<<" ";
    if(end_!=cap_) {
        // cout << begin_ <<" "<<cap_;
        allocator_.construct((end_),value);
        ++end_;
        len_ ++;
        // cout<<end_<<" ";
    }
}

template <class T>
void vector<T>::pop_back() {
    // TODO exception
    allocator_.destroy(end_ - 1);
    --end_;
    len_--;
}

}

#endif

