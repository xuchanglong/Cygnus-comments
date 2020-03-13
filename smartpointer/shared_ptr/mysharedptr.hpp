#include <functional>

template <typename T>
class MySharedPtr
{
public:
    /**
     * 普通构造函数。    
    */
    MySharedPtr(T *ptr = nullptr, const std::function<void(T *)> &del = Deleter) : pmem(ptr),
                                                                                   puse_count(new std::size_t(ptr != nullptr)),
                                                                                   deleter(del)
    {
    }

    /**
     * 拷贝构造函数。
     * 让本 this 和 rhs 相关变量相同。
    */
    MySharedPtr(const MySharedPtr &rhs) : puse_count(rhs.puse_count),
                                          pmem(rhs.pmem),
                                          deleter(rhs.deleter)
    {
        ++*puse_count;
    }

    /**
     * 赋值运算符。
    */
    MySharedPtr &operator=(const MySharedPtr &rhs)
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

public:
    std::size_t use_count()
    {
        return *puse_count;
    }

    bool unique() const
    {
        return *puse_count == 1;
    }

    operator bool() const
    {
        return pmem != nullptr;
    }

    void swap(MySharedPtr &rhs)
    {
        using std::swap;
        swap(puse_count, rhs.puse_count);
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
        if (--*puse_count == 0)
        {
            if (pmem)
            {
                deleter(pmem);
            }
            delete puse_count;
        }
        puse_count = nullptr;
        pmem = nullptr;
    }

    static void Deleter(T *p)
    {
        delete p;
    }

private:
    // pmem 指向的内存的计数。
    std::size_t *puse_count;

    // 指向被管理的内容。
    T *pmem;

    // 内存释放器。
    std::function<void(T *)> deleter;
};