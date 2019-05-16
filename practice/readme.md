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

为了精密分工，STL allocator 决定将两阶段操作区分开，内存配置操作由 alloc::allocate()负责，内存释放操作由alloc::deallocate()负责；
对象构造操作由 ::construct()负责，对象析构操作由::destory()负责

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
#### [TODO]4.4.4 deque数据结构
#### 4.4.5 deque的构造与内存管理 ctor,push_back,push_front

# Realization

