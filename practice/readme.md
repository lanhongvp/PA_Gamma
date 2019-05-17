# STL Practice
## 实现一个内存空间配置的五个功能
- 内存空间请求
- 内存容量扩充（加一倍处理）
- 内存块空间释放
- 容器元素对象创建
- 容器元素对象释放
# MATERIAL
## 0 pre知识
### 0.1 size_t/ptrdiff_t数据类型
- size_t/ptrdiff_t数据类型是和机器相关的
- size_t是无符号整数
    - 表达指针地址值
- ptrdiff_t是有符号整数
    - 表达指针地址的差值

```cpp
// Definitions of common types
#ifdef _WIN64
    typedef unsigned __int64    size_t;
    typedef __int64             ptrdiff_t;
    typedef __int64             intptr_t;
#else
    typedef unsigned int        size_t;
    typedef int                 ptrdiff_t;
    typedef int                 intptr_t;
#endif
```

## 2 空间配置器
### 2.1 空间配置器的标准接口
根据STL规范，以下是allocator的必要接口
- allocator::value_type
- allocator::pointer
- allocator::const_pointer
- allocator::reference
- allocator::const_reference
- allocator::size_type
- allocator::difference_type
- allocator::rebind
    - 一个嵌套的（nested）class template
    - TODO P76
- allocator::allocator()
    - default constructor
- allocator::allocator(const allocator&)
    - copy constructor(因为传引用？TODO)
- template <class U>allocator::allocator(const allocator<U>&)
    - 泛化的copy constructor
- allocator::~allocator()
    - TODO
- pointer allocator::address(reference x) const
    - 返回某个对象的地址
- const_pointer allocator::address(const_reference x) const
    - 返回某个const对象的地址
- pointer allocator::allocate(size_type n,const void* =0)
    - 配置空间，足以存储n个T对象，第二个参数仅做提示
- void allocator::deallocate(pointer p,size_type n)
    - 归还先前配置的空间
- size_type allocator::max_size() const
    - 返回可成功配置的最大量
- void allocator::construct(pointer p,const T& x)
    - 等同于new((const void*) p) T(x)
- void allocator::destroy(pointer p)
    - 等同于p->~T()
#### 2.1.1 设计一个简单的空间配置器 JJ.allocator
根据上述标准接口，可以自行完成一个功能简单，接口不完全齐全的allocator
- TODO 模板类的使用语法
### 2.2 具备次配置力的SGI空间配置器
#### 2.2.1 SGI标准的空间配置器，std::allocator
SGI也定义有一个符合部分标准，名为allocator的配置器，但效率不佳
#### 2.2.2 SGI特殊的空间配置器，std::alloc
一般而言，C++内存配置操作和释放操作：
```cpp
class Foo{...};
Foo* pf = new Foo;
delete pf;
```

- new算式内含两阶段操作
    - 调用 ::operator new 配置内存
    - 调用 Foo::Foo() 构造对象内容
- delete算式内含两阶段操作
    - 调用 Foo::~Foo() 将对象析构
    - 调用 ::operator delete 释放内存

为了精密分工，STL allocator **决定将两阶段操作区分开，内存配置操作由 alloc::allocate()负责，内存释放操作由alloc::deallocate()负责；
对象构造操作由 ::construct()负责，对象析构操作由::destory()负责**

STL标准规格表明，配置器定义于<memory>中，SGI<memory>内含以下两个文件
```cpp
#include <stl_alloc.h>          //负责内存空间的配置和释放
#include <stl_construct.h>      //负责对象内容的构造与析构
```

```
<memory>
    - <stl_alloc.h>
    - <stl_construct.h>
    - <stl_uninitialized.h>
```

#### 2.2.3 构造和析构基本工具
- TODO construct()和destory()
- TODO trivial destructor
#### 2.2.4 空间的配置与释放 std::alloc
SGI设计了双层级配置器
- 第一级配置器直接使用malloc(),free()
- 第二级配置器采用不同的策略
#### 2.2.5 第一级配置器 __malloc_alloc_template剖析
- TODO template<int inst>
    - 第一级配置器以malloc(),free(),realloc()等C函数执行实际的内存配置、释放、重配置操作，并实现类似C++ new-handler的机制
#### [TODO]2.2.6 第二级配置器 __default_alloc_template剖析
#### [TODO]2.2.7 空间配置函数
#### [TODO]2.2.8 空间释放函数
#### [TODO]2.2.9 重新填充free lists
#### [TODO]2.2.10 内存池
### [TODO]2.3 内存基本处理工具
## 3 迭代器概念与traits编程技法
### 3.1 迭代器设计思维--STL关键所在
STL的中心思想在于：将数据容器和算法分开，彼此独立设计，最后进行撮合
- 容器和算法的泛型化：class templates和function templates
```cpp
// <stl_algo.h>
template <class InputIterator,class T>
InputIterator find(InputIterator first,InputIterator last,                      const T& value) {
    while(first != last && *first != value) {
        ++first;
    }
    return first;
}
```
### 3.2 迭代器是一种smart pointer
指针各种行为中最常见也最重要的便是内容提领(dereference)和成员访问(member access)
- 迭代器最重要的编程工作即是对operator*和operator->进行重载
    - TODO auto_ptr
#### 为list(链表)设计一个迭代器
- 需要为list设计一个类似指针的外衣，即迭代器，当derefernce这一迭代器时，传回的应该是ListItem对象；当递增该迭代器时，应该指向下一个ListItem对象
- TODO 模板类和模板函数，template <class/typename> x

```cpp
template <typename T>
class List {
    void insert_front(T value);
    void insert_end(T value);
    void display(std::ostream &os = std::cout) const;
private:
    ListItem<T>* _end;
    ListItem<T>* _front;
    long _size;
};

template <typename T>
class ListItem {
public: 
    T value() const {return _value;}
    ListItem* next() const {return _next;}
private:
    T _value;
    ListItem* _next;
};
```

- TODO copy ctor/ctor P116
- RTTI Run Time Type Identification

### 3.3 迭代器相应型别
- 利用`function template`的参数推导机制
### 3.4 Traits编程技法--STL源代码门钥
迭代器所指对象的型别，称为该迭代器的value type，当value_type必须用于函数的传回值，即束手无策，函数的“template参数推导机制”只是参数，无法推导函数的返回值型别
- 声明内嵌型别

```cpp
template <class T>
struct MyIter {
    typedef T value_type;   //内嵌型别声明(nested type)
    T* ptr;
    MyIter(T* p=0) : ptr(p) { }
    T& operator*() const {return *ptr;}
    //...
};

template <class I>
typename I::value_type   //这一整行是func的返回值类型
func(I iter) {
    return *iter;
}

//...
MyIter<int> iter(new int(8));
cout << func(ite);
```
- func()返回型别前须加上关键词`typename`
- 隐藏陷阱：并不是所有迭代器都是class type,原生指针就不是，若不是class type，即无法定义内嵌型别
#### partial specialization
如果`class template`拥有一个以上的template参数，可以针对某个template参数进行特化工作

```cpp
template <class I>
struct iterator_traits {
    typedef typename I::value_type value_type;
};
```
- 如果I定义有自己的value type，通过traits的作用，得到的value_type就是I::value_type
```cpp
template <class I>
typename iterator_traits<I>::value_type
func(I ite) {
    return *ite;
}
```
- 相比与上一个版本，多了一层间接性，且traits可以拥有特化版本
- 注意：针对指向常数对象的指针(pointer-to-const)
```cpp
template <class T>
struct iterator_traits<const T*> {
    typedef T value_type;
};
```
- 最常用到的迭代器相应型别有五种
#### 3.4.1 迭代器相应型别一：value_type
所谓value_type，即指迭代器所指对象的型别，任何一个打算与STL算法完美搭配的class，都应该定义自己的value type内嵌型别
#### 3.4.2 迭代器相应型别二：difference type
difference type用来表示两个迭代器之间的距离，因此其可以表示一个容器的最大容量，因为对于连续空间而言，头尾之间的距离就是其最大容量
#### 3.4.3 迭代器相应型别三：reference type
C++中，函数如果要传回左值，都是以by reference方式进行，所以当p是mutable iterators，如果其value type是T，*p应该为T&
#### 3.4.4 迭代器相应型别四：pointer type
- TODO P92 针对原生指针设计的偏特化版 

#### 3.4.5 迭代器相应型别五：iterator_category
根据移动特性与施行操作，迭代器被分为五类
- input iterator
- output iterator: write only
- forward iterator
- bidirectional iterator
- random access iterator

以advance()为例，该函数有两个参数，迭代器p和数值n，函数内部将p累进n次，针对不同的iterator，普通情况下advance()实现了三份定义，在泛型思维的设计下，需要三者合一，设计思路如下：
- 如果traits有能力萃取出迭代器的种类，便可以利用此迭代器相应型别作为advance()的第三个参数，使得函数重载机制得以有效运作。重新设计__advance()，并加上第三参数，使其形成重载
    - TODO `iterator_traits<Iterator>::iterator_category()`将产生一个暂时对象

## 4 序列式容器
### 4.2 vector
#### 4.2.2 vector定义摘要
#### [TODO]4.2.3 vector的迭代器
#### 4.2.4 vector的数据结构
vector所采用的数据结构非常简单：线性连续空间，以两个迭代器start和finish分别指向配置得来的连续空间中目前已被使用的范围，并以迭代器end_of_storage指向整块连续空间的尾端
#### 4.2.5 vector的构造与内存管理：constructor,push_back
- TODO vector缺省使用alloc(作为空间配置器)，并据此另外定义了一个data_allocator，为的是更方便以元素大小为配置单位 P154
- 动态增加大小，并不是在原空间之后接续新空间，而是以原大小的两倍另外配置一块较大空间，然后将原内容拷贝过来，然后才开始在原内容之后构造新元素，并释放原空间
    - 对vector的任何操作，一旦引起空间重新配置，指向原vector的所有迭代器即失效
#### [TODO]4.2.6 vector元素操作
### 4.4 deque
#### 4.4.2 deque中控器
deque是连续空间，deque采用一块map(一小块连续空间)作为主控，map中的每个元素都是指针，指向另一段(较大的)连续现行空间，称为缓冲区
```cpp
template <class T,class Alloc=alloc,size_t BufSize=0>
class deque {
public:
    typedef T value_type;
    typedef value_type* pointer;
    ...
protected:
    // 元素指针的指针
    typedef pointer* map_pointer;

protected:
    map_pointer map;  //指向map,map是块连续空间，其内的每个元素都是一个指针（称为节点），指向一块缓冲区
    size_type map_size; //map可容纳多少指针

};
```
#### 4.4.3 deque的迭代器
deque是分段连续空间，维持其整体连续的任务落在了迭代器operator++和operator--两个运算子身上
- 理解deque中控器、缓冲区、迭代器的相互关系
#### 4.4.4 deque数据结构
- deque除了维护一个先前说过的指向map的指针外，也维护start,finish连个迭代器，分别指向第一缓冲区的第一个元素和最后缓冲区的最后一个元素(的下一个位置)
- 记住当前map的大小，一旦map所提供的节点不足，须重新配置更大的一块map
#### 4.4.5 deque的构造与内存管理 ctor,push_back,push_front

# Realization

