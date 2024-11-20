#include <iostream>
#include <endianness.hpp>

void testEndianness()
{
    nstd::endianness end;

    // Test 16-bit conversions
    std::uint16_t value16 = 0x1234;
    std::uint16_t net16 = end.htons(value16);
    std::uint16_t host16 = end.ntohs(net16);
    std::cout << "16-bit conversion:" << std::endl;
    std::cout << "Original: 0x" << std::hex << value16 << std::endl;
    std::cout << "Network:  0x" << net16 << std::endl;
    std::cout << "Host:     0x" << host16 << std::endl << std::endl;

    // Test 32-bit conversions
    std::uint32_t value32 = 0x12345678;
    std::uint32_t net32 = end.htonl(value32);
    std::uint32_t host32 = end.ntohl(net32);
    std::cout << "32-bit conversion:" << std::endl;
    std::cout << "Original: 0x" << value32 << std::endl;
    std::cout << "Network:  0x" << net32 << std::endl;
    std::cout << "Host:     0x" << host32 << std::endl << std::endl;

    // Test 64-bit conversions
    std::uint64_t value64 = 0x123456789ABCDEF0;
    std::uint64_t net64 = end.htonll(value64);
    std::uint64_t host64 = end.ntohll(net64);
    std::cout << "64-bit conversion:" << std::endl;
    std::cout << "Original: 0x" << value64 << std::endl;
    std::cout << "Network:  0x" << net64 << std::endl;
    std::cout << "Host:     0x" << host64 << std::endl;
}

int main()
{
    testEndianness();
    return 0;
}