#include <cstddef>
#include <algorithm>

#include <cassert>
#define SHARED_ASSERT(x) assert(x)

template <typename T>
inline T &move(T &v)
{
    return v;
}

template <class T>
class unique_ptr
{
public:
    typedef T element_type;

    unique_ptr(void) throw() : px(NULL)
    {
    }
    /**
     * 普通构造函数。
    */
    explicit unique_ptr(T *p) throw() : px(p)
    {
    }
    /**
     * 拷贝构造函数。
    */
    unique_ptr(const unique_ptr &ptr) throw() : px(ptr.px)
    {
        const_cast<unique_ptr &>(ptr).px = NULL;
    }
    /**
     * 赋值构造函数。
    */
    unique_ptr &operator=(unique_ptr ptr) throw()
    {
        swap(ptr);
        return *this;
    }

    inline ~unique_ptr(void) throw()
    {
        destroy();
    }
    /**
     * 销毁被管理的内存。
    */
    inline void reset(void) throw()
    {
        destroy();
    }
    /**
     * 更新被管理的内存，之前的内存被 delete 。
    */
    void reset(T *p) throw()
    {
        SHARED_ASSERT((NULL == p) || (px != p));
        destroy();
        px = p;
    }
    /**
     * 交换两个 unique_ptr 所管理的内存。
    */
    void swap(unique_ptr &lhs) throw()
    {
        std::swap(px, lhs.px);
    }
    /**
     * 释放 unique_ptr 对内存的管理权。
    */
    inline void release(void) throw()
    {
        px = NULL;
    }

    inline operator bool() const throw()
    {
        return (NULL != px);
    }

    inline T &operator*() const throw()
    {
        SHARED_ASSERT(NULL != px);
        return *px;
    }
    inline T *operator->() const throw()
    {
        SHARED_ASSERT(NULL != px);
        return px;
    }
    inline T *get(void) const throw()
    {
        return px;
    }

private:
    inline void destroy(void) throw()
    {
        delete px;
        px = NULL;
    }

    inline void release(void) const throw()
    {
        px = NULL;
    }

private:
    /**
     * 保存被管理的内存的首地址。
    */
    T *px;
};

template <class T, class U>
inline bool operator==(const unique_ptr<T> &l, const unique_ptr<U> &r) throw()
{
    return (l.get() == r.get());
}
template <class T, class U>
inline bool operator!=(const unique_ptr<T> &l, const unique_ptr<U> &r) throw()
{
    return (l.get() != r.get());
}
template <class T, class U>
inline bool operator<=(const unique_ptr<T> &l, const unique_ptr<U> &r) throw()
{
    return (l.get() <= r.get());
}
template <class T, class U>
inline bool operator<(const unique_ptr<T> &l, const unique_ptr<U> &r) throw()
{
    return (l.get() < r.get());
}
template <class T, class U>
inline bool operator>=(const unique_ptr<T> &l, const unique_ptr<U> &r) throw()
{
    return (l.get() >= r.get());
}
template <class T, class U>
inline bool operator>(const unique_ptr<T> &l, const unique_ptr<U> &r) throw()
{
    return (l.get() > r.get());
}
