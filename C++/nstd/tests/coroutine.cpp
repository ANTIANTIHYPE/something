#include <coroutine.hpp>

#if __cplusplus > 201703L

nstd::coroutine sampleCoroutine()
{
    std::cout << "Coroutine started." << std::endl;
    co_yield 1;
    std::cout << "Coroutine resumed, yielding 2." << std::endl;
    co_yield 2;
    std::cout << "Coroutine resumed, returning 3." << std::endl;
    co_return 3;
}

void testCoroutineC20()
{
    std::cout << "Testing C++20 coroutine..." << std::endl;
    nstd::coroutine coro = sampleCoroutine();

    while (coro.resume())
    {
        std::cout << "Yielded value: " << coro.current_value() << std::endl;
    }
    std::cout << "Return value: " << coro.return_value() << std::endl;
}

#else // ^^^ C++20 ^^^ VVV C++98 to C++17 VVV

void testCoroutineC17()
{
    std::cout << "Testing C++17 coroutine..." << std::endl;

    nstd::coroutine coro([]()
    {
        std::cout << "Coroutine started." << std::endl;
        for (int i = 1; i <= 3; ++i)
        {
            std::cout << "Yielding: " << i << std::endl;
            if (i < 3)
            {
                throw std::exception();
            }
        }
    });

    try
    {
        while (coro.resume())
        {
            std::cout << "Coroutine resumed." << std::endl;
        }
    }
    catch (const std::exception&)
    {
        std::cout << "Coroutine finished." << std::endl;
    }
}

#endif // ^^^ C++98 to C++17 ^^^

int main()
{
#if __cplusplus > 201703L

    testCoroutineC20();

#else // ^^^ C++20 ^^^ VVV C++98 to C++17 VVV

    testCoroutineC17();

#endif // ^^^ C++98 to C++17 ^^^
    return 0;
}