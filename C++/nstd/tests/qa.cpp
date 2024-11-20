#include <qa.hpp>

void test()
{
    try
    {
        nstd::QuantumArray quantumArray(3);

        quantumArray.set(0, {1.0, 0.0});       // |0>
        quantumArray.set(1, {0.0, 1.0});       // |1>
        quantumArray.set(2, {0.7071, 0.7071}); // |+>

        std::cout << "Initial Quantum States:" << std::endl;
        quantumArray.print();

        std::cout << "Measuring quantum state..." << std::endl;
        quantumArray.measure();

        std::cout << "Normalizing quantum states..." << std::endl;
        quantumArray.normalize();
        quantumArray.print();

        std::cout << "Measuring quantum state after normalization..." << std::endl;
        quantumArray.measure();

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    try
    {
        nstd::QuantumArray invalidArray(0);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        nstd::QuantumArray outOfRangeArray(2);
        outOfRangeArray.set(0, {1.0, 0.0});
        outOfRangeArray.get(2);
    }
    catch (const std::out_of_range& e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        nstd::QuantumArray zeroNormArray(2);
        zeroNormArray.normalize();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }
}

int main()
{
    test();
    return 0;
}