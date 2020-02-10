class counter
{
public:
    counter() : w(0){};
    int w;
};

template <class T>
class weak_ptr
{
public: 
    weak_ptr()
    {
        _ptr = nullptr;
        cnt = nullptr;
    }
    weak_ptr(shared_ptr<T> &ptr) : _ptr(ptr._ptr), cnt(ptr.cnt)
    {
        cnt->w++;
    }
    weak_ptr(weak_ptr<T> &ptr) : _ptr(ptr._ptr), cnt(ptr.cnt)
    {
        cnt->w++;
    }
    ~weak_ptr()
    {
        release();
    }
    weak_ptr<T> &operator=(weak_ptr<T> &ptr)
    {
        if (this != &ptr)
        {
            release();
            cnt = ptr.cnt;
            cnt->w++;
            _ptr = ptr._ptr;
        }
        return *this;
    }
    weak_ptr<T> &operator=(shared_ptr<T> &ptr)
    {
        release();
        cnt = ptr.cnt;
        cnt->w++;
        _ptr = ptr._ptr;
        return *this;
    }
    shared_ptr<T> lock()
    {
        return shared_ptr<T>(*this);
    }
    bool expired()
    {
        if (cnt)
        {
            if (cnt->w > 0)
            {
                return false;
            }
        }
        return true;
    }
    friend class shared_ptr<T>; //方便weak_ptr与share_ptr设置引用计数和赋值。
private:
    void release()
    {
        if (cnt)
        {
            cnt->w--;
            if (cnt->w < 1)
            {
                cnt = nullptr;
            }
        }
    }
    T *_ptr;
    counter *cnt;
};