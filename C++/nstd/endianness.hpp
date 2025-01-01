#pragma once

#include <cstdint>

namespace nstd
{

/**
 * @brief A class for handling endianness conversions.
 * 
 * This class provides methods to convert 16-bit, 32-bit, and 64-bit integers
 * between host byte order and network byte order (big-endian).
 */
namespace endianness
{

/**
 * @brief Converts a 16-bit integer from host to network byte order.
 * 
 * @param value The 16-bit integer to convert.
 * 
 * @return The converted 16-bit integer in network byte order.
 */
constexpr inline std::uint16_t htons(std::uint16_t value)
{
    return (value << 8) | (value >> 8);
}

/**
 * @brief Converts a 32-bit integer from host to network byte order.
 * 
 * @param value The 32-bit integer to convert.
 * 
 * @return The converted 32-bit integer in network byte order.
 */
constexpr inline std::uint32_t htonl(std::uint32_t value)
{
    return ((value << 24) & 0xFF000000) | // Move byte 0 to byte 3
    ((value << 8)  & 0x00FF0000) |        // Move byte 1 to byte 2
    ((value >> 8)  & 0x0000FF00) |        // Move byte 2 to byte 1
    ((value >> 24) & 0x000000FF);         // Move byte 3 to byte 0
}

/**
 * @brief Converts a 64-bit integer from host to network byte order.
 * 
 * @param value The 64-bit integer to convert.
 * 
 * @return The converted 64-bit integer in network byte order.
 */
constexpr inline std::uint64_t htonll(std::uint64_t value)
{
    return ((value << 56) & 0xFF00000000000000ULL) | // Move byte 0 to byte 7
    ((value << 40) & 0x00FF000000000000ULL) |        // Move byte 1 to byte 6
    ((value << 24) & 0x0000FF0000000000ULL) |        // Move byte 2 to byte 5
    ((value << 8)  & 0x000000FF00000000ULL) |        // Move byte 3 to byte 4
    ((value >> 8)  & 0x00000000FF000000ULL) |        // Move byte 4 to byte 3
    ((value >> 24) & 0x0000000000FF0000ULL) |        // Move byte 5 to byte 2
    ((value >> 40) & 0x000000000000FF00ULL) |        // Move byte 6 to byte 1
    ((value >> 56) & 0x00000000000000FFULL);         // Move byte 7 to byte 0
}

}; // namespace endianness

} // namespace nstd
