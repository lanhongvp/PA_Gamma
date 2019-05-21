#ifndef TINYSTL_LIST_H_
#define TINYSTL_LIST_H_

#include <initializer_list>

#include "alloc.h"

namespace lanstl {
    // list node design
    template <class T>
    struct _list_node {
        typedef _list_node<T>* void_pointer;
        _list_node(const T& val) {
            pre = nullptr;
            next = nullptr;
            data = val;
        }
        void_pointer pre;
        void_pointer next;
        T data;
    };

    template <class T>
    class list {
    public:
        typedef _list_node<T> list_node;
        // typedef lanstl::allocator<T> node_allocator;
        typedef lanstl::allocator<list_node>    node_allocator;
        typedef list_node*                      pointer;
        typedef T                               value_type;
        typedef T&                              reference;
        typedef size_t                          size_type;
        typedef ptrdiff_t                       difference_type;
    
    private:
        node_allocator allocator_;
        list_node*     front_;
        list_node*     end_;
        size_type      size_;     

    public:
        // construct
        list(size_type n, const value_type& val) {
            size_ = n;
            front_ = end_ = nullptr;
            while(n--) {
                fill_init(n,val);
            }
        }

        // deconstruct
        ~list() {
            destroy_l(end_,size_);
            front_ = end_ = nullptr;
            size_ = 0;
        }

        void push_back(const value_type& val);
        void push_front(const value_type& val);
        void pop_back();
        void pop_front();

        size_type size() const {
            return static_cast<size_type>(size_);
        }

        reference operator[](size_type n) {
            return find_index(n)->data;

        }
        
        pointer find_index(size_type n) {
            list_node* p = front_;
            while((p!=nullptr) && (n-->0)) {
                p = (list_node*)(p->next);
            }
            return p;
        }

    private:
        void fill_init(size_type n,const value_type& value);
        void destroy_l(pointer cur,size_type n);
        void unlink_nodes(pointer first,pointer last);
        // list_node* find_index(size_type n);

    };

// fill_init
template <class T>
void list<T> :: fill_init(size_type n,const value_type& val) {
    list_node* new_end = allocator_.allocate(1);
    allocator_.construct(new_end,val);

    if(front_==nullptr) {
        front_ = end_ = new_end;
    } else {
        end_->next = new_end;
        new_end->pre = end_;
        // new_end->next = nullptr;
        end_ = new_end;
    }
}

// destroy_l
template <class T>
void list<T>::destroy_l(pointer cur,size_type n) {
    while(n--) {
        allocator_.destroy(cur);
        allocator_.deallocate(cur);
    }
}

// push_back
template <class T>
void list<T>::push_back(const value_type& val) {
    list_node* new_end = allocator_.allocate(1);
    allocator_.construct(new_end, val);
    if(front_==nullptr) {
        front_ = end_ = nullptr;
    } else {
        end_->next = new_end;
        new_end->pre = end_;
        new_end->next = nullptr;
        end_ = new_end; 
    }
    size_++;
}

// push_front
template <class T>
void list<T>::push_front(const value_type& val) {
    list_node* new_front = allocator_.allocate(1);
    allocator_.construct(new_front, val);
    if(front_==nullptr) {
        front_ = end_ = nullptr;
    } else {
        front_->pre = new_front;
        // new_front->pre = nullptr;
        new_front->next = front_;
        front_ = new_front; 
    } 
    size_++;
}

// lost link in list
template <class T>
void list<T>::unlink_nodes(pointer first, pointer last)
{
    first->pre->next = last->next;
    last->next->pre = first->pre;
}

// pop_back
template <class T>
void list<T>::pop_back() {
    // cout << end_;
    if(front_ == nullptr) {
        return;
    }
    pointer new_end = end_->pre;
    // unlink_nodes(new_end,new_end);
    allocator_.destroy(end_);
    allocator_.deallocate(end_);
    end_ = new_end;
    if(end_ == nullptr) {
        front_ == nullptr;
    }
    size_--;
}

template <class T> 
void list<T>::pop_front() {
    if(front_==nullptr) {
        return;
    }
    pointer new_front = front_->next;
    allocator_.destroy(front_);
    allocator_.deallocate(front_);
    front_ = new_front;
    size_--;
}

}

#endif