#include "mysharedptr.hpp"
#include <iostream>
#include <memory>

int main()
{
    MySharedPtr<int> i11(new int(11));
    MySharedPtr<int> i21(new int(12));
    MySharedPtr<int> i12(i11);
    MySharedPtr<int> i13(i12);

    std::cout << "i21 use_count = " << i21.use_count() << std::endl;
    std::cout << "i21 = " << *i21.get() << std::endl;
    
    i21 = i12;

    std::cout << "i12 use_count = " << i12.use_count() << std::endl;
    std::cout << "i12 = " << *i12.get() << std::endl;

    std::cout << "i21 use_count = " << i21.use_count() << std::endl;
    std::cout << "i21 = " << *i21.get() << std::endl;

    std::cout << "--------------------------------------------" << std::endl;

    std::shared_ptr<int> t11(new int(11));
    std::shared_ptr<int> t21(new int(12));
    std::shared_ptr<int> t12(t11);
    std::shared_ptr<int> t13(t12);

    std::cout << "t21 use_count = " << t21.use_count() << std::endl;
    std::cout << "t21 = " << *t21.get() << std::endl;
    
    t21 = t12;

    std::cout << "t12 use_count = " << t12.use_count() << std::endl;
    std::cout << "t12 = " << *t12.get() << std::endl;

    std::cout << "t21 use_count = " << t21.use_count() << std::endl;
    std::cout << "t21 = " << *t21.get() << std::endl;
    return 0;
}