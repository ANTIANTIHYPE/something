#include <iostream>
#include <array>
#include <iomanip>
#include <aes.hpp>

void printArray(const std::uint8_t* array, std::size_t size) 
{
    for (size_t i = 0; i < size; ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(array[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}

void test()
{
    nstd::aes aes;

    std::array<std::uint8_t, 16> input = { 0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37,
                                           0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34 };
    std::array<std::uint8_t, 16> key = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                         0xab, 0xf7, 0x97, 0x67, 0x45, 0x61, 0x4c, 0x2b };
    std::array<std::uint8_t, 16> encryptedOutput;
    std::array<std::uint8_t, 16> decryptedOutput;

    aes.encrypt(input.data(), key.data(), encryptedOutput.data());
    aes.decrypt(encryptedOutput.data(), key.data(), decryptedOutput.data());

    std::cout << "Key:              ";
    printArray(key.data(), key.size());
    std::cout << "Input:            ";
    printArray(input.data(), input.size());
    std::cout << "Encrypted Output: ";
    printArray(encryptedOutput.data(), encryptedOutput.size());
    std::cout << "Decrypted Output: ";
    printArray(decryptedOutput.data(), decryptedOutput.size());
}

int main()
{
    test();
    return 0;
}