#include <functional>

template <typename T>
class MySharedPtr
{
public:
    /**
     * 普通构造函数。    
    */
    MySharedPtr(T *ptr = nullptr, const std::function<void(T *)> &del = Deleter) : pmem(ptr),
                                                                                   pcount_(new std::size_t(ptr != nullptr)),
                                                                                   deleter(del)
    {
    }

    /**
     * 拷贝构造函数。
     * 让本 this 和 rhs 相关变量相同。
    */
    MySharedPtr(const MySharedPtr &rhs) : pcount_(rhs.pcount_),
                                          pmem(rhs.pmem),
                                          deleter(rhs.deleter)
    {
        ++*pcount_;
    }

    /**
     * 赋值运算符。
    */
    MySharedPtr &operator=(MySharedPtr rhs)
    {

        MySharedPtr<T> temp(rhs);
        // 此处执行 swap 目的是将 rhs 的 pcount 减一。
        swap(temp);
        return *this;
    }

    ~MySharedPtr()
    {
        release();
    }

    std::size_t use_count()
    {
        return *pcount_;
    }

    bool unique() const
    {
        return *pcount_ == 1;
    }

    operator bool() const
    {
        return pmem != nullptr;
    }

    void swap(MySharedPtr &rhs)
    {
        using std::swap;
        swap(pcount_, rhs.pcount_);
        swap(pmem, rhs.pmem);
        swap(deleter, rhs.deleter);
    }

    void reset(T *ptr = nullptr, const std::function<void(T *)> &del = Deleter())
    {
        pmem = ptr;
        deleter = del;
    }

    T *get() const
    {
        return pmem;
    }

    T &operator*() const
    {
        return *pmem;
    }
    T *operator->() const
    {
        return pmem;
    }

private:
    void release()
    {
        if (--*pcount_ == 0)
        {
            if (pmem)
            {
                deleter(pmem);
            }
            delete pcount_;
        }
        pcount_ = nullptr;
        pmem = nullptr;
    }

    static void Deleter(T *p)
    {
        delete p;
    }

private:
    // pmem 指向的内存的计数。
    std::size_t *pcount_;
    T *pmem;
    std::function<void(T *)> deleter;
};