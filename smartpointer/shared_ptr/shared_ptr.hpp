#ifndef CYGNUS_COMMENTS_SMARTPOINTER_SHARED_PTR_H_
#define CYGNUS_COMMENTS_SMARTPOINTER_SHARED_PTR_H_

#include <algorithm> // std::swap
#include <cassert>
#define SHARED_ASSERT(x) assert(x)

/**
 * 与 shared_ptr 类配套的内存引用计数器。
*/
class shared_ptr_count
{
public:
    shared_ptr_count() : pn_(nullptr)
    {
    }

    shared_ptr_count(const shared_ptr_count &countobj) : pn_(countobj.pn_)
    {
    }

    /**
     * 交换两个 shared_ptr_count 对应的引用计数器的值。
    */
    void swap(shared_ptr_count &countobj) throw()
    {
        std::swap(pn_, countobj.pn_);
    }

    /**
     * 返回该内存的引用计数。
    */
    long use_count(void) const throw()
    {
        long count = 0;
        if (nullptr != pn_)
        {
            count = *pn_;
        }
        return count;
    }

    /**
     * 内存计数器 + 1 。
    */
    template <class U>
    void countadd(U *p)
    {
        if (nullptr != p)
        {
            if (nullptr == pn_)
            {
                try
                {
                    pn_ = new long(1);
                }
                catch (std::bad_alloc &)
                {
                    delete p;
                    throw;
                }
            }
            else
            {
                ++(*pn_);
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
        if (nullptr != pn_)
        {
            --(*pn_);
            if (0 == *pn_)
            {
                delete p;
                delete pn_;
            }
            pn_ = nullptr;
        }
    }

public:
    /**
     * 指向该内存的引用计数器。
    */
    long *pn_;
};

template <class T>
class shared_ptr
{
public:
    typedef T element_type;

    /**
     * 普通构造函数。
    */
    shared_ptr(void) throw() : px_(nullptr),
                               countobj_()
    {
    }

    explicit shared_ptr(T *p) : countobj_()
    {
        countadd(p);
    }

    /**
     * 拷贝构造函数。
    */
    template <class U>
    shared_ptr(const shared_ptr<U> &ptr, T *p) : countobj_(ptr.pn_)
    {
        countadd(p);
    }

    template <class U>
    shared_ptr(const shared_ptr<U> &ptr) throw() : countobj(ptr.pn)
    {
        SHARED_ASSERT((nullptr == ptr.px) || (0 != ptr.countobj.use_count()));
        acquire(static_cast<typename shared_ptr<T>::element_type *>(ptr.px));
    }

    shared_ptr(const shared_ptr &ptr) throw() : countobj(ptr.pn)
    {
        SHARED_ASSERT((nullptr == ptr.px) || (0 != ptr.countobj.use_count()));
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
        SHARED_ASSERT((nullptr == p) || (px_ != p));
        release();
        countadd(p);
    }

    /**
     * 交换两个 shared_ptr 中的内容。
    */
    void swap(shared_ptr &countobj) throw()
    {
        std::swap(px_, countobj.px_);
        countobj.swap(countobj.pn_);
    }

    operator bool() const throw()
    {
        return (0 < countobj.use_count());
    }
    bool unique(void) const throw()
    {
        return (1 == countobj.use_count());
    }
    long use_count(void) const throw()
    {
        return countobj.use_count();
    }

    T &operator*() const throw()
    {
        SHARED_ASSERT(NULL != px_);
        return *px_;
    }
    T *operator->() const throw()
    {
        SHARED_ASSERT(NULL != px_);
        return px_;
    }

    /**
     * 返回内存的首地址。
    */
    T *get(void) const throw()
    {
        return px_;
    }

private:
    /**
     * 内存的引用计数 + 1 。
    */
    void countadd(T *p)
    {
        countobj_.countadd(p);
        px_ = p;
    }

    /**
     * 释放对内存的控制权。
    */
    void release(void) throw()
    {
        countobj_.release(px_);
        px_ = nullptr;
    }

private:
    template <class U>
    friend class shared_ptr;

private:
    /**
     * 保存被管理的内存的首地址。
    */
    T *px_;

    /**
     * px 指向的内存的引用计数器。
    */
    shared_ptr_count countobj_;
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

#endif