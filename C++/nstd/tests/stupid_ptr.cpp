#include <iostream>
#include <cassert>
#include <stupid_ptr.hpp>

class Sample
{
public:
    int value;

    Sample(int v) : value(v) {}
};

void test()
{
    Sample* obj = new Sample(42);

    nstd::stupid_ptr<Sample> sp(obj);

    assert((*sp).value == 42);

    assert(sp->value == 42);

    (*sp).value = 100;
    assert((*sp).value == 100);

    delete obj;

    std::cout << "All tests passed." << std::endl;
}

int main()
{
    test();
    return 0;
}