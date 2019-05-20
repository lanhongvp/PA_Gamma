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
private:
    iterator        begin_;
    iterator        end_;
    iterator        cap_;      

public:
    //construct
    vector(size_type n,const value_type& value) {
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

private:
    // some helper functions
    void fill_init(size_type n,const value_type& value);
    void destroy_v(iterator first,iterator last,size_type n);

};

// fill_init
template <class T>
void vector<T>:: fill_init(size_type n, const value_type& value) {
    fill_n(begin_,n,value);
}

template <class iter,class size,class T>
iter fill_n(iter first,size n,const T& val) {
    for(;n>0;--n,++first) {
        *first = val;
    }
    return first;
}

// destroy_v
template <class T>
void vector<T>:: destroy_v(iterator first,iterator last,size_type n) {
    data_allocator::destroy(first,last);
    data_allocator::deallocate(first,n);
}

// push_back
// TODO 私有变量的访问范围
template <class T> 
void vector<T>::push_back(const value_type& value) {
    if(end_!=cap_) {
        data_allocator::construct(lanstl::address_of(*end_),value);
        ++end_;
    }
}

template <class T>
void vector<T>::pop_back() {
    // TODO exception
    data_allocator::destroy(end_ - 1);
    --end_;
}

}

#endif

