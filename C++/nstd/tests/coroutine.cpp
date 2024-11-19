#include <coroutine.hpp>

#if __cplusplus > 201703L

nstd::coroutine sampleCoroutine()
{
    std::cout << "Coroutine started.\n";
    co_yield 1;
    std::cout << "Coroutine resumed, yielding 2.\n";
    co_yield 2;
    std::cout << "Coroutine resumed, returning 3.\n";
    co_return 3;
}

void testCoroutineC20()
{
    std::cout << "Testing C++20 coroutine:\n";
    nstd::coroutine coro = sampleCoroutine();

    while (coro.resume())
    {
        std::cout << "Yielded value: " << coro.current_value() << '\n';
    }
    std::cout << "Return value: " << coro.return_value() << '\n';
}

#else // ^^^ C++20 ^^^ VVV C++98 to C++17 VVV

void testCoroutineC17()
{
    std::cout << "Testing C++17 coroutine:\n";

    nstd::coroutine coro([]()
    {
        std::cout << "Coroutine started.\n";
        for (int i = 1; i <= 3; ++i)
        {
            std::cout << "Yielding: " << i << '\n';
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
            std::cout << "Coroutine resumed.\n";
        }
    }
    catch (const std::exception&)
    {
        std::cout << "Coroutine finished.\n";
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