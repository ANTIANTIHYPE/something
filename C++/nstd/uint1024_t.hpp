
#include <array>
#include <cstdint>
#include <stdexcept>
#include <iomanip>

namespace nstd
{
    /**
     * @brief A class representing a 1024-bit unsigned integer.
     */
    class uint1024_t
    {
    public:
        static const std::size_t SIZE = 16;
        std::array<std::uint64_t, SIZE> data;

        /**
         * @brief Default constructor initializes the number to zero.
         */
        constexpr uint1024_t() noexcept
        {
            data.fill(0);
        }

        /**
         * @brief Constructor that initializes the number with a given 64-bit value.
         * @param value The 64-bit value to initialize the number.
         */
        constexpr uint1024_t(std::uint64_t value) noexcept
        {
            data.fill(0);
            data[0] = value;
        }

        /**
         * @brief Adds two uint1024_t numbers.
         * 
         * @param other The number to add.
         * @return The sum of the two numbers.
         */
        constexpr uint1024_t operator+(const uint1024_t& other) const
        {
            uint1024_t result;
            std::uint64_t carry = 0;

            for (std::size_t i = 0; i < SIZE; ++i)
            {
                std::uint64_t sum = data[i] + other.data[i] + carry;
                result.data[i] = sum;
                carry = (sum < data[i]) || (carry && sum == data[i]);
            }

            return result;
        }

        /**
         * @brief Subtracts one uint1024_t number from another.
         * 
         * @param other The number to subtract.
         * @return The result of the subtraction.
         * @throws std::invalid_argument if the result is negative.
         */
        constexpr uint1024_t operator-(const uint1024_t& other) const
        {
            uint1024_t result;
            std::uint64_t borrow = 0;

            for (std::size_t i = 0; i < SIZE; ++i)
            {
                std::uint64_t sub = data[i] - other.data[i] - borrow;
                result.data[i] = sub;
                borrow = (sub > data[i]);
            }

            return result;
        }

        /**
         * @brief Multiplies two uint1024_t numbers.
         * 
         * @param other The number to multiply with.
         * @return The product of the two numbers.
         */
        constexpr uint1024_t operator*(const uint1024_t& other) const
        {
            uint1024_t result;
            for (std::size_t i = 0; i < SIZE; ++i)
            {
                if (other.data[i] == 0) continue;
                uint1024_t temp = *this;
                for (std::size_t j = 0; j < i; ++j)
                {
                    temp.leftShift();
                }
                result = result + (temp * other.data[i]);
            }
            return result;
        }

        /**
         * @brief Divides this number by another uint1024_t number.
         * 
         * @param other The divisor.
         * @return The quotient of the division.
         * @throws std::invalid_argument if dividing by zero.
         */
        constexpr uint1024_t operator/(const uint1024_t& other) const
        {
            if (other.isZero())
            {
                throw std::invalid_argument("Division by zero");
            }

            uint1024_t quotient, remainder;
            for (int i = SIZE * 64 - 1; i >= 0; --i)
            {
                remainder.leftShift();
                remainder.data[0] |= (data[i / 64] >> (i % 64)) & 1;

                if (remainder >= other)
                {
                    remainder = remainder - other;
                    quotient.setBit(i);
                }
            }

            return quotient;
        }

        /**
         * @brief Computes the modulus of this number by another uint1024_t number.
         * 
         * @param other The divisor.
         * @return The remainder of the division.
         * @throws std::invalid_argument if the modulus is by zero.
         */
        constexpr uint1024_t operator%(const uint1024_t& other) const
        {
            if (other.isZero())
            {
                throw std::invalid_argument("Modulus by zero");
            }

            uint1024_t quotient = *this / other;
            uint1024_t product = quotient * other;
            uint1024_t remainder = *this - product;

            return remainder;
        }

        /**
         * @brief Performs a bitwise AND operation with another uint1024_t number.
         * 
         * @param other The number to AND with.
         * @return The result of the bitwise AND operation.
         */
        constexpr uint1024_t operator&(const uint1024_t& other) const
        {
            uint1024_t result;
            for (std::size_t i = 0; i < SIZE; ++i)
            {
                result.data[i] = data[i] & other.data[i];
            }
            return result;
        }

        /**
         * @brief Performs a bitwise OR operation with another uint1024_t number.
         * 
         * @param other The number to OR with.
         * @return The result of the bitwise OR operation.
         */
        constexpr uint1024_t operator|(const uint1024_t& other) const
        {
            uint1024_t result;
            for (std::size_t i = 0; i < SIZE; ++i)
            {
                result.data[i] = data[i] | other.data[i];
            }
            return result;
        }

        /**
         * @brief Performs a bitwise XOR operation with another uint1024_t number.
         * 
         * @param other The number to XOR with.
         * @return The result of the bitwise XOR operation.
         */
        constexpr uint1024_t operator^(const uint1024_t& other) const
        {
            uint1024_t result;
            for (std::size_t i = 0; i < SIZE; ++i)
            {
                result.data[i] = data[i] ^ other.data[i];
            }
            return result;
        }

        /**
         * @brief Left shifts the number by one bit and assigns the result.
         * @return A reference to this object.
         */
        constexpr uint1024_t& operator<<=(int shift) noexcept
        {
            for (int i = 0; i < shift; ++i)
            {
                leftShift();
            }
            return *this;
        }

        /**
         * @brief Right shifts the number by one bit and assigns the result.
         * @return A reference to this object.
         */
        constexpr uint1024_t& operator>>=(int shift) noexcept
        {
            for (int i = 0; i < shift; ++i)
            {
                rightShift();
            }
            return *this;
        }

        /**
         * @brief Performs a three-way comparison with another uint1024_t number.
         * 
         * @param other The number to compare with.
         * @return A comparison result.
         */
        constexpr std::strong_ordering operator<=>(const uint1024_t& other) const
        {
            for (size_t i = SIZE; i-- > 0;)
            {
                if (data[i] < other.data[i])
                {
                    return std::strong_ordering::less;
                }
                if (data[i] > other.data[i])
                {
                    return std::strong_ordering::greater;
                }
            }
            return std::strong_ordering::equal;
        }

        /**
         * @brief Left shifts the number by one bit.
         */
        constexpr void leftShift() noexcept
        {
            for (size_t i = SIZE - 1; i > 0; --i)
            {
                data[i] = (data[i] << 1) | (data[i - 1] >> 63);
            }
            data[0] <<= 1;
        }

        /**
         * @brief Right shifts the number by one bit.
         */
        constexpr void rightShift() noexcept
        {
            for (size_t i = 0; i < SIZE - 1; ++i)
            {
                data[i] = (data[i] >> 1) | (data[i + 1] << 63);
            }
            data[SIZE - 1] >>= 1;
        }

        /**
         * @brief Sets a specific bit in the number.
         * 
         * @param index The index of the bit to set (0 to 1023).
         * @throws std::out_of_range if the index is out of range.
         */
        constexpr void setBit(size_t index)
        {
            if (index >= SIZE * 64)
            {
                throw std::out_of_range("Index out of range");
            }
            data[index / 64] |= (1ULL << (index % 64));
        }

        /**
         * @brief Checks if the number is zero.
         * @return true if the number is zero, false otherwise.
         */
        constexpr bool isZero() const
        {
            for (const auto& part : data)
            {
                if (part != 0)
                {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Outputs the number as a stream.
         * 
         * @param os The output stream.
         * @param value The uint1024_t number to output.
         * @return The output stream.
         */
        constexpr friend std::ostream& operator<<(std::ostream& os, const uint1024_t& value) noexcept
        {
            for (size_t i = 0; i < uint1024_t::SIZE; ++i)
            {
                os << value.data[i];
                if (i < uint1024_t::SIZE - 1)
                {
                    os << ", ";
                }
            }
            return os;
        }

        /**
         * @brief Compares this number with another for less than.
         * 
         * @param other The number to compare with.
         * @return True if this number is less than the other, false otherwise.
         */
        constexpr bool operator<(const uint1024_t& other) const
        {
            for (std::size_t i = SIZE; i-- > 0;)
            {
                if (data[i] != other.data[i])
                {
                    return data[i] < other.data[i];
                }
            }
            return false;
        }

        /**
         * @brief Compares this number with another for greater than.
         * 
         * @param other The number to compare with.
         * @return True if this number is greater than the other, false otherwise.
         */
        constexpr bool operator>(const uint1024_t& other) const
        {
            return other < *this;
        }

        /**
         * @brief Compares this number with another for less than or equal to.
         * 
         * @param other The number to compare with.
         * @return True if this number is less than or equal to the other, false otherwise.
         */
        constexpr bool operator<=(const uint1024_t& other) const
        {
            return !(*this > other);
        }

        /**
         * @brief Compares this number with another for greater than or equal to.
         * 
         * @param other The number to compare with.
         * @return True if this number is greater than or equal to the other, false otherwise.
         */
        constexpr bool operator>=(const uint1024_t& other) const
        {
            return !(*this < other);
        }

        /**
         * @brief Compares this number with another for equality.
         * 
         * @param other The number to compare with.
         * @return true if the numbers are equal, false otherwise.
         */
        constexpr bool operator==(const uint1024_t& other) const
        {
            for (std::size_t i = 0; i < SIZE; ++i)
            {
                if (data[i] != other.data[i])
                {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Compares this number with another for inequality.
         * 
         * @param other The number to compare with.
         * @return true if the numbers are not equal, false otherwise.
         */
        constexpr bool operator!=(const uint1024_t& other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Converts the number to a hexadecimal string representation.
         * @return A string representing the number in hexadecimal format.
         */
        constexpr std::string toHex() const
        {
            std::ostringstream oss;
            for (std::size_t i = SIZE; i-- > 0;)
            {
                oss << std::hex << std::setw(16) << std::setfill('0') << data[i];
            }
            return oss.str();
        }
    }; // class uint1024_t
} // namespace nstd
