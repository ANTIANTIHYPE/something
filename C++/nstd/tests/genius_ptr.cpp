#include <iostream>
#include <cassert>
#include <genius_ptr.hpp>

class Sample
{
public:
    Sample(int value) : value(value) {}
    int getValue() const { return value; }
private:
    int value;
};

void testDefaultConstructor()
{
    nstd::genius_ptr<Sample> ptr;
    assert(ptr.get() == nullptr);
    assert(ptr.use_count() == 0);
    std::cout << "testDefaultConstructor passed." << std::endl;
}

void testConstructorWithPointer()
{
    nstd::genius_ptr<Sample> ptr(new Sample(42));
    assert(ptr.get() != nullptr);
    assert(ptr.use_count() == 1);
    assert(ptr->getValue() == 42);
    std::cout << "testConstructorWithPointer passed." << std::endl;
}

void testCopyAssignment()
{
    nstd::genius_ptr<Sample> ptr1(new Sample(42));
    nstd::genius_ptr<Sample> ptr2;
    ptr2 = ptr1;
    assert(ptr1.use_count() == 1);
    assert(ptr2.use_count() == 2);
    assert(ptr1->getValue() == ptr2->getValue());
    std::cout << "testCopyAssignment passed." << std::endl;
}

void testMoveAssignment()
{
    nstd::genius_ptr<Sample> ptr1(new Sample(42));
    nstd::genius_ptr<Sample> ptr2;
    ptr2 = std::move(ptr1);
    assert(ptr1.get() == nullptr);
    assert(ptr2.use_count() == 1);
    assert(ptr2->getValue() == 42);
    std::cout << "testMoveAssignment passed." << std::endl;
}

void testNullptrAssignment()
{
    nstd::genius_ptr<Sample> ptr(new Sample(42));
    ptr = nullptr;
    assert(ptr.get() == nullptr);
    assert(ptr.use_count() == 0);
    std::cout << "testNullptrAssignment passed." << std::endl;
}

void testWeakPtr()
{
    nstd::genius_ptr<Sample> ptr1(new Sample(42));
    nstd::genius_ptr<Sample>::weak_ptr weakPtr = ptr1.get_weak_ptr();
    assert(!weakPtr.expired());
    
    nstd::genius_ptr<Sample> ptr2 = weakPtr.lock();
    assert(ptr2.get() != nullptr);
    assert(ptr2->getValue() == 42);
    
    ptr1 = nullptr;
    assert(weakPtr.expired());
    assert(weakPtr.lock().get() == nullptr);
    std::cout << "testWeakPtr passed." << std::endl;
}

int main()
{
    testDefaultConstructor();
    testConstructorWithPointer();
    testCopyAssignment();
    testMoveAssignment();
    testNullptrAssignment();
    testWeakPtr();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}