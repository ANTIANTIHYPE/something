#include <iostream>
#include <genius_ptr.hpp>

class Sample
{
public:
    constexpr Sample(int value) : value(value)
    {
        std::cout << "Sample constructed with value: " << value << '\n';
    }

    ~Sample()
    {
        std::cout << "Sample destructed with value: " << value << '\n';
    }

    constexpr void display() const 
    {
        std::cout << "Value: " << value << '\n';
    }

private:
    int value;
};

int main()
{
    nstd::genius_ptr<Sample> ptr1(new Sample(69));
    ptr1->display();

    nstd::genius_ptr<Sample> ptr2 = ptr1;
    std::cout << "Use count after copying ptr1 to ptr2: " << ptr1.use_count() << '\n';

    nstd::genius_ptr<Sample>::weak_ptr weakPtr = ptr1.get_weak_ptr();
    std::cout << "Weak pointer expired: " << weakPtr.expired() << '\n';

    if (!weakPtr.expired())
    {
        nstd::genius_ptr<Sample> ptr3 = weakPtr.lock();
        ptr3->display();
    }

    ptr1 = nullptr;
    std::cout << "Use count after resetting ptr1: " << ptr2.use_count() << '\n';

    ptr2 = nullptr;

    return 0;
}