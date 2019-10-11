#pragma once

#include <algorithm> // std::swap

#include <cassert>
#define SHARED_ASSERT(x) assert(x)

/**
 * 与 shared_ptr 类配套的内存引用计数器。
*/
class shared_ptr_count
{
public:
    shared_ptr_count() : pn(nullptr)
    {
    }

    shared_ptr_count(const shared_ptr_count &count) : pn(count.pn)
    {
    }

    /**
     * 交换两个 shared_ptr_count 对应的引用计数器的值。
    */
    void swap(shared_ptr_count &lhs) throw()
    {
        std::swap(pn, lhs.pn);
    }

    /**
     * 返回该内存的引用计数。
    */
    long use_count(void) const throw()
    {
        long count = 0;
        if (NULL != pn)
        {
            count = *pn;
        }
        return count;
    }

    /**
     * 内存计数器+ 1 。
    */
    template <class U>
    void acquire(U *p)
    {
        if (NULL != p)
        {
            if (NULL == pn)
            {
                try
                {
                    pn = new long(1);
                }
                catch (std::bad_alloc &)
                {
                    delete p;
                    throw;
                }
            }
            else
            {
                ++(*pn);
            }
        }
    }

    /**
     * 释放 share_ptr 对 p 指定的内存的控制权。
     * 若 p 指定的内存没有其他 share_ptr 使用，
     * 则直接释放内存。
    */
    template <class U>
    void release(U *p) throw()
    {
        if (NULL != pn)
        {
            --(*pn);
            if (0 == *pn)
            {
                delete p;
                delete pn;
            }
            pn = NULL;
        }
    }

public:
    /**
     * 指向该内存的引用计数器。
    */
    long *pn;
};

template <class T>
class shared_ptr
{
public:
    typedef T element_type;

    /**
     * 普通构造函数。
    */
    shared_ptr(void) throw() : px(NULL),
                               pn()
    {
    }

    explicit shared_ptr(T *p) : pn()
    {
        acquire(p);
    }

    /**
     * 拷贝构造函数。
    */
    template <class U>
    shared_ptr(const shared_ptr<U> &ptr, T *p) : pn(ptr.pn)
    {
        acquire(p);
    }

    template <class U>
    shared_ptr(const shared_ptr<U> &ptr) throw() : pn(ptr.pn)
    {
        SHARED_ASSERT((NULL == ptr.px) || (0 != ptr.pn.use_count()));
        acquire(static_cast<typename shared_ptr<T>::element_type *>(ptr.px));
    }

    shared_ptr(const shared_ptr &ptr) throw() : pn(ptr.pn)
    {
        SHARED_ASSERT((NULL == ptr.px) || (0 != ptr.pn.use_count()));
        acquire(ptr.px);
    }

    /**
     * 赋值构造函数。
    */
    shared_ptr &operator=(shared_ptr ptr) throw()
    {
        swap(ptr);
        return *this;
    }

    ~shared_ptr(void) throw()
    {
        release();
    }

    void reset(void) throw()
    {
        release();
    }

    void reset(T *p)
    {
        SHARED_ASSERT((NULL == p) || (px != p));
        release();
        acquire(p);
    }

    /**
     * 交换两个 shared_ptr 中的内容。
    */
    void swap(shared_ptr &lhs) throw()
    {
        std::swap(px, lhs.px);
        pn.swap(lhs.pn);
    }

    operator bool() const throw()
    {
        return (0 < pn.use_count());
    }
    bool unique(void) const throw()
    {
        return (1 == pn.use_count());
    }
    long use_count(void) const throw()
    {
        return pn.use_count();
    }

    T &operator*() const throw()
    {
        SHARED_ASSERT(NULL != px);
        return *px;
    }
    T *operator->() const throw()
    {
        SHARED_ASSERT(NULL != px);
        return px;
    }

    /**
     * 返回内存的首地址。
    */
    T *get(void) const throw()
    {
        return px;
    }

private:
    /**
     * 内存的引用计数 + 1 。
    */
    void acquire(T *p)
    {
        pn.acquire(p);
        px = p;
    }

    /**
     * 释放对内存的控制权。
    */
    void release(void) throw()
    {
        pn.release(px);
        px = NULL;
    }

private:
    template <class U>
    friend class shared_ptr;

private:
    /**
     * 保存被管理的内存的首地址。
    */
    T *px;

    /**
     * px 指向的内存的引用计数器。
    */
    shared_ptr_count pn;
};

template <class T, class U>
bool operator==(const shared_ptr<T> &l, const shared_ptr<U> &r) throw() 
{
    return (l.get() == r.get());
}
template <class T, class U>
bool operator!=(const shared_ptr<T> &l, const shared_ptr<U> &r) throw() 
{
    return (l.get() != r.get());
}
template <class T, class U>
bool operator<=(const shared_ptr<T> &l, const shared_ptr<U> &r) throw() 
{
    return (l.get() <= r.get());
}
template <class T, class U>
bool operator<(const shared_ptr<T> &l, const shared_ptr<U> &r) throw() 
{
    return (l.get() < r.get());
}
template <class T, class U>
bool operator>=(const shared_ptr<T> &l, const shared_ptr<U> &r) throw() 
{
    return (l.get() >= r.get());
}
template <class T, class U>
bool operator>(const shared_ptr<T> &l, const shared_ptr<U> &r) throw() 
{
    return (l.get() > r.get());
}

template <class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U> &ptr) 
{
    return shared_ptr<T>(ptr, static_cast<typename shared_ptr<T>::element_type *>(ptr.get()));
}

template <class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &ptr) 
{
    T *p = dynamic_cast<typename shared_ptr<T>::element_type *>(ptr.get());
    if (NULL != p)
    {
        return shared_ptr<T>(ptr, p);
    }
    else
    {
        return shared_ptr<T>();
    }
}
