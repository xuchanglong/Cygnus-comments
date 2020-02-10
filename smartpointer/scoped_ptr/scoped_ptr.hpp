template <class T>
class scoped_ptr
{
private:
    T *ptr;

    scoped_ptr(scoped_ptr const &);
    scoped_ptr &operator=(scoped_ptr const &);

    typedef scoped_ptr<T> this_type;

public:
    typedef T element_type;

    /**
     * 构造函数，存储 p 的一份拷贝。注意，p 必须是用operator new分配的，或者是null。在构造的时候，
     * 不要求 T 必须是一个完整的类型。当指针 p 是调用某个allocation函数的结果而不是直接调用 new 得到的时
     * 候很有用：因为这个类型不必是完整的，只需要类型T的一个前置声明就可以了。这个构造函数不会抛出异常。
    */
    explicit scoped_ptr(T *p = 0) : ptr(p)
    {
    }

    /**
     * 从一个auto_ptr手中接过控制权，指向auto_ptr指针所指的对象，并且auto_prt指针不再指向该对象。
    */
    explicit scoped_ptr(std::auto_ptr<T> p) : ptr(p.release())
    {
    }

    /**
     * 删除所指对象。类型T在被销毁时必须是一个完整的类型。如果scoped_ptr在它被析构时并没有
     * 占有资源，它就什么都不做。这个析构函数不会抛出异常。
    */
    ~scoped_ptr()
    {
        /**
           template<typename T>
           inline void checked_delete(T *x)
           {
               typedef char type_must_be_complete[sizeof(T)];
               delete x;
           }

           or

           template<typename T>
           inline void checked_delete(T *x)
           {
               typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
               (void) sizeof(type_must_be_complete);
               
               delete x;
           }
        */
        boost::checked_delete(ptr);
    }

    /**
     * 重置一个 scoped_ptr 就是删除它已保存的指针，如果它有的话，并重新保存 p。 通常，资源的生存期
     * 管理应该完全由scoped_ptr自己处理，但是在极少数时候，资源需要在scoped_ptr的析构之前释放，或
     * 者scoped_ptr要处理它原有资源之外的另外一个资源。这时，就可以用reset，但一定要尽量少用它。
     * (过多地使用它通常表示有设计方面的问题) 这个函数不会抛出异常。
    */
    void reset(T *p = 0)
    {
        ((p == 0 || p != ptr) ? (void)0 : _assert("reset error"));
        this_type(p).swap(*this);
    }

    /**
     * 返回一个到被保存指针指向的对象的引用。由于不允许空的引用，所以解引用一个拥有空指针
     * 的 scoped_ptr 将导致未定义行为。如果不能肯定所含指针是否有效，就用函数 get 替代解引用。
     * 这个函数不会抛出异常。
    */
    T &operator*() const
    {
        ((ptr != 0) ? (void)0 : _assert("operator * error"));
        return *ptr;
    }

    /**
     * 返回保存的指针。如果保存的指针为空，则调用这个函数会导致未定义行为。如果不能确定指针是否
     * 空的，最好使用函数get。这个函数不会抛出异常。
    */
    T *operator->() const
    {
        ((ptr != 0) ? (void)0 : _assert("operator -> error");
        return ptr;
    }

    /**
     * 返回保存的指针。应该小心地使用get，因为它可以直接操作裸指针。但是，get使得你可以测试保存的
     * 指针是否为空。这个函数不会抛出异常。get通常用于调用那些需要裸指针的函数。
    */
    T *get() const
    {
        return ptr;
    }

    typedef T *this_type::*unspecified_bool_type;

    /**
     * 返回scoped_ptr是否为非空。返回值的类型是未指明的，但这个类型可被用于Boolean的上下文中。
     * 在if语句中最好使用这个类型转换函数，而不要用get去测试scoped_ptr的有效性
    */
    operator unspecified_bool_type() const
    {
        return ptr == 0 ? 0 : &this_type::ptr;
    }

    bool operator!() const
    {
        return ptr == 0;
    }

    /**
     * 交换两个scoped_ptr的内容。这个函数不会抛出异常。
    */
    void swap(scoped_ptr &b)
    {
        T *tmp = b.ptr;
        b.ptr = ptr;
        ptr = tmp;
    }
};

/**
 * 这个函数提供了交换两个scoped pointer的内容的更好的方法。之所以说它更好，是因为
 * swap(scoped1,scoped2) 可以更广泛地用于很多指针类型，包括裸指针和第三方的智能指针。
 * [2] scoped1.swap(scoped2) 则只能用于它的定义所在的智能指针，而不能用于裸指针。
*/
template <class T>
inline void swap(scoped_ptr<T> &a, scoped_ptr<T> &b)
{
    a.swap(b);
}

template <class T>
inline T *get_pointer(scoped_ptr<T> const &p)
{
    return p.get();
}