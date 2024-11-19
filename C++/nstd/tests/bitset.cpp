#include <iostream>
#include <bitset.hpp>

int main()
{
    try
    {
        nstd::bitset bitset1("1101"); // Represents: 1, 1, 0, 1
        std::cout << "bitset1: " << bitset1.toString() << std::endl;

        nstd::bitset bitset2{true, false, true, false}; // Represents: 1, 0, 1, 0
        std::cout << "bitset2: " << bitset2.toString() << std::endl;

        nstd::bitset andResult = bitset1 & bitset2;
        std::cout << "bitset1 & bitset2: " << andResult.toString() << std::endl;

        nstd::bitset orResult = bitset1 | bitset2;
        std::cout << "bitset1 | bitset2: " << orResult.toString() << std::endl;

        nstd::bitset xorResult = bitset1 ^ bitset2;
        std::cout << "bitset1 ^ bitset2: " << xorResult.toString() << std::endl;

        bitset1.resize(6);
        std::cout << "Resized bitset1: " << bitset1.toString() << std::endl;

        bitset1.setBit(4, true);
        std::cout << "bitset1 after setting bit 4: " << bitset1.toString() << std::endl;

        bool bitValue = bitset1.getBit(2);
        std::cout << "Value of bit 2 in bitset1: " << (bitValue ? '1' : '0') << std::endl;

    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}