#ifndef TINYSTL_LIST_H_
#define TINYSTL_LIST_H_

#include <initializer_list>

#include "alloc.h"

namespace lanstl {
    // 五种迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    
    // list node design
    template <class T>
    struct _list_node {
        typedef void* void_pointer;
        _list_node(const T& val) {
            pre = nullptr;
            next = nullptr;
            data = val;
        }
        void_pointer pre;
        void_pointer next;
        T data;
    };

    template <class T,class ref,class ptr>
    struct _list_iter {
        typedef _list_iter<T,T&,T*> iterator;
        typedef _list_iter<T,ref,ptr> self;

        typedef bidirectional_iterator_tag  iterator_category;
        typedef T value_type;
        typedef ptr pointer;
        typedef ref reference;
        typedef _list_node<T>* link_type;
        typedef size_t  size_type;
        typedef ptrdiff_t difference_type;

        link_type node;

        // constructor
        _list_iter() {}

        reference operator*() const {
            return (*node).data;
        }
    };
    template <class T>
    class list {
    public:
        typedef _list_node<T> list_node;
        // typedef lanstl::allocator<T> node_allocator;
        typedef lanstl::allocator<list_node> node_allocator;
        typedef T value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        list(size_type n, const value_type& val) {
            front_ = end_ = nullptr;
            while(--n) {
                push_back(val);
            }
        }

        void push_back(const value_type& val) {
            list_node* new_end = allocator_.allocate(1);
            allocator_.construct(new_end, val);
            if(front_ == nullptr) {
                // empty list
                front_ = end_ = new_end;
            } else {
                end_->next = new_end;
                new_end->pre = end_;
                end_ = new_end;
            }
            
        }

        void push_front(const value_type& val) {
            list_node* new_front = allocator_.allocate(1);
            allocator_.construct(new_front, val);
            if(front == nullptr) {
                front_ = end_ = new_end;
            } else {
                front_->pre = new_front;
                new_front->next = front_;
                front_ = new_front;
            }
        }

        value_type& operator[](size_type n) {
            return find_index(n)->data;
        }

        list_node* find_index(size_type n) {
            list_node* p = front_;
            while((p != nullptr) && (n-- > 0)) {
                p = (list_node*)(p->next);
            }
            return p;
        }

        void pop_back() {
            if(front_ = nullptr) {
                return;
            }
            list_node* new_end = end_->pre;
            allocator_.destroy(end_);
            allocator_.deallocate(end_, 1);
            end_ = new_end;
            if(end_ == nullptr) {
                front_ = nullptr;
            }
        }



    private:
        node_allocator allocator_;
        list_node*     front_;
        list_node*     end_;
    };

}

#endif