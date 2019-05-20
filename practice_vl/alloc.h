#ifndef _TINYSTL_ALLOC_
#define _TINYSTL_ALLOC_

#include <new>
#include <cstddef>     // for ptrdiff_t,size_t
#include <cstdlib>     // for exit()
#include <climits>     // for UINT_MAX
#include <iostream>

using namespace std;


namespace lanstl {

    template <class T>  //typename
    inline T* _allocate(ptrdiff_t size,T*) {
        set_new_handler(0);
        // :: 调用全局new 
        T* tmp = (T*)(::operator new((size_t)(size*sizeof(T))));
        if(tmp==0) {
            cerr << "out of memory" << endl;
            exit(1);
        }
        return tmp;
    }

    template <class T> 
    inline void _deallocate(T* buffer) {
        ::operator delete(buffer);
    }

    template <class T1,class T2> 
    inline void _construct(T1* p,const T2 & val) {
        new(p) T1(val);  //在程序员指定的空间里new,同时调用拷贝构造函数
    }

    template <class T>
    inline void _destroy_one(T* ptr) {
        ptr->~T();
    }

    template <class T>
    inline void _destroy_cat(T* first,T* last) {
        for(;first!=last;++first) {
            _destroy_one(&*first);
        }
    }

    template <class T>
    class allocator {
    public:
        typedef T       value_type;
        typedef T*      pointer;
        typedef const T*    const_pointer;
        typedef T&      reference;
        typedef const T&      const_reference;
        typedef size_t        size_type;
        typedef ptrdiff_t     difference_type;
        // typedef value_type*   iterator;

        template <class U>
        // don't know the meaning
        struct rebind {
            typedef allocator<U> other;
        };

        pointer allocate(size_type n,const void* hint=0) {
            return _allocate((difference_type)n,(pointer)0);
        }

        void deallocate(pointer p,size_type n) {_deallocate(p);}

        void construct(pointer p,const T& val) {
            _construct(p,val);
        }

        static void destroy(T* ptr);
        static void destroy(T* first, T* last);
        // void destroy(pointer p) {_destroy(p);}
        // void destroy(pointer first,pointer last) {_destroy_cat(first,last);}
        
        pointer address(reference x) {return (pointer)& x;}

        // const_pointer const_address(const_reference x) {
        //     return (const_pointer)&x;
        // }

        size_type max_size() const {
            return size_type(UINT_MAX/sizeof(T));
        }
        
    };

template <class T>
void allocator<T>::destroy(T* ptr)
{
  _destroy_one(ptr);
}

template <class T>
void allocator<T>::destroy(T* first, T* last)
{
  _destroy_cat(first, last);
}

}

#endif //_LANALLOC_