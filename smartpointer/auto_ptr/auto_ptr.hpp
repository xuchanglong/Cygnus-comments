#ifndef SMARTPOINTER_AUTO_PTR_AUTO_PTR_HPP_
#define SMARTPOINTER_AUTO_PTR_AUTO_PTR_HPP_

template <class X>
class auto_ptr
{
private:
    /**
     * 指向被管理的内存。
    */
    X *ptr;

    /**
     * 记录该 auto_ptr 是否拥有对 ptr 指向的内存的所有权。
    */
    mutable bool owns;

public:
    typedef X element_type;

    /**
     * 普通构造函数。
    */
    explicit auto_ptr(X *p = 0) : ptr(p), owns(p) {}

    /**
     * 拷贝构造函数。
    */
    auto_ptr(const auto_ptr &a) : ptr(a.ptr), owns(a.owns)
    {
        a.owns = 0;
    }
    template <class T>
    auto_ptr(const auto_ptr<T> &a)
        : ptr(a.ptr),
          owns(a.owns)
    {
        a.owns = 0;
    }

    /**
     * 赋值构造函数。
    */
    auto_ptr &operator=(const auto_ptr &a)
    {
        if (&a != this)
        {
            if (owns)
                delete ptr;
            owns = a.owns;
            ptr = a.ptr;
            a.owns = 0;
        }
    }
    template <class T>
    auto_ptr &operator=(const auto_ptr<T> &a)
    {
        if (&a != this)
        {
            if (owns)
                delete ptr;
            owns = a.owns;
            ptr = a.ptr;
            a.owns = 0;
        }
    }

    ~auto_ptr()
    {
        /**
         * 只有拥有了内存所有权的 auto_ptr 才能释放该内存，
         * 避免 delete 过期指针。
        */
        if (owns)
            delete ptr;
    }

    /**
     * 运算符重载函数。
    */
    X &operator*() const { return *ptr; }
    X *operator->() const { return ptr; }

    /**
     * 返回所管理的内存的首地址。
    */
    X *get() const { return ptr; }

    /**
     * 释放对 ptr 指向的内存的所有权并返回该内存首地址。
    */
    X *release() const
    {
        owns = false;
        return ptr;
    }
};

#endif