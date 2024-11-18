#pragma once

#include <stdexcept>
#include <cstdint>
#include <vector>

namespace nstd
{
    /**
     * @brief A dynamic bitset implementation.
     * 
     * The `bitset` class allows manipulation of a sequence of bits with support for
     * bitwise operations, resizing, and initialization from binary strings or
     * initializer lists.
     */
    class bitset
    {
    public:
        /**
         * @brief Constructs a bitset from a binary string.
         * 
         * @param bit_str A string representing the bits, consisting of '0's and '1's.
         * @throws std::runtime_error if the string is empty or contains invalid characters.
         */
        constexpr explicit bitset(const std::string& bit_str)
        {
            if (bit_str.empty())
            {
                throw std::runtime_error("Empty bitset");
            }
            size_ = bit_str.size();
            data_.resize((size_ + 63) / 64, 0);

            for (std::size_t i = 0; i < size_; ++i)
            {
                if (bit_str[i] == '1')
                {
                    data_[i / 64] |= (1ULL << (i % 64));
                }
                else if (bit_str[i] != '0')
                {
                    throw std::runtime_error("Invalid bitset string");
                }
            }
        }

        /**
         * @brief Constructs an empty bitset of a given size.
         * @param size The size of the bitset in bits.
         */
        constexpr explicit bitset(std::size_t size) : size_(size), data_((size + 63) / 64, 0) {}

        /**
         * @brief Constructs a bitset from an initializer list of booleans.
         * @param bits An initializer list representing the bits.
         */
        constexpr bitset(std::initializer_list<bool> bits) : size_(bits.size()), data_((size_ + 63) / 64, 0)
        {
            std::size_t index = 0;
            for (bool bit : bits)
            {
                setBit(index++, bit);
            }
        }

        /**
         * @brief Retrieves the value of a bit at a given position.
         * 
         * @param pos The position of the bit to retrieve.
         * @return true if the bit is set, false otherwise.
         * @throws std::runtime_error if the position is out of range.
         */
        constexpr bool getBit(std::size_t pos) const
        {
            if (pos >= size_)
            {
                throw std::runtime_error("Bit position out of range");
            }
            return (data_[pos / 64] >> (pos % 64)) & 1;
        }

        /**
         * @brief Sets the value of a bit at a given position.
         * 
         * @param pos The position of the bit to set.
         * @param value The value to set the bit to (true for 1, false for 0).
         * @throws std::runtime_error if the position is out of range.
         */
        constexpr void setBit(std::size_t pos, bool value)
        {
            if (pos >= size_)
            {
                throw std::runtime_error("Bit position out of range");
            }
            if (value)
            {
                data_[pos / 64] |= (1ULL << (pos % 64));
            }
            else
            {
                data_[pos / 64] &= ~(1ULL << (pos % 64));
            }
        }

        /**
         * @brief Converts the bitset to a string representation.
         * @return A string representing the bits, with '1's and '0's.
         */
        constexpr std::string toString() const
        {
            std::string result(size_, '0');
            for (std::size_t i = 0; i < size_; ++i)
            {
                if (getBit(i))
                {
                    result[i] = '1';
                }
            }
            return result;
        }

        /**
         * @brief Resizes the bitset to a new size.
         * @param newSize The new size of the bitset in bits.
         */
        constexpr void resize(std::size_t newSize)
        {
            if (newSize == size_)
            {
                return;
            }
            size_ = newSize;
            data_.resize((newSize + 63) / 64, 0);
        }

        /**
         * @brief Performs a bitwise AND operation with another bitset.
         * 
         * @param other The other bitset to AND with.
         * @return A new bitset representing the result of the AND operation.
         * @throws std::runtime_error if the sizes of the bitsets do not match.
         */
        constexpr bitset operator&(const bitset& other) const
        {
            if (size_ != other.size_)
            {
                throw std::runtime_error("Bitsets must be of the same size for AND operation");
            }
            bitset result(size_);
            for (std::size_t i = 0; i < data_.size(); ++i)
            {
                result.data_[i] = data_[i] & other.data_[i];
            }
            return result;
        }

        /**
         * @brief Performs a bitwise OR operation with another bitset.
         * 
         * @param other The other bitset to OR with.
         * @return A new bitset representing the result of the OR operation.
         * @throws std::runtime_error if the sizes of the bitsets do not match.
         */
        constexpr bitset operator|(const bitset& other) const
        {
            if (size_ != other.size_)
            {
                throw std::runtime_error("Bitsets must be of the same size for OR operation");
            }
            bitset result(size_);
            for (std::size_t i = 0; i < data_.size(); ++i)
            {
                result.data_[i] = data_[i] | other.data_[i];
            }
            return result;
        }

        /**
         * @brief Performs a bitwise XOR operation with another bitset.
         * 
         * @param other The other bitset to XOR with.
         * @return A new bitset representing the result of the XOR operation.
         * @throws std::runtime_error if the sizes of the bitsets do not match.
         */
        constexpr bitset operator^(const bitset& other) const
        {
            if (size_ != other.size_)
            {
                throw std::runtime_error("Bitsets must be of the same size for XOR operation");
            }
            bitset result(size_);
            for (std::size_t i = 0; i < data_.size(); ++i)
            {
                result.data_[i] = data_[i] ^ other.data_[i];
            }
            return result;
        }

        /**
         * @brief Checks if the bitset is empty (size is zero).
         * @return true if the bitset is empty, false otherwise.
         */
        constexpr bool empty() const
        {
            return size_ == 0;
        }

        /**
         * @brief Retrieves the size of the bitset.
         * @return The size of the bitset in bits.
         */
        constexpr std::size_t size() const
        {
            return size_;
        }

    private:
        std::size_t size_;
        std::vector<std::uint64_t> data_;
    }; // class bitset

    /**
     * @brief Exception thrown for invalid bitset operations.
     * 
     * The `invalid_bitset` class extends std::runtime_error to indicate that
     * a provided string or operation does not conform to valid bitset rules.
     */
    class invalid_bitset : public std::runtime_error
    {
    public:
        /**
         * @brief Constructs an invalid_bitset exception with a message.
         * @param what A description of the invalid operation or string.
         */
        invalid_bitset(const std::string& what) : std::runtime_error(what + " is not a valid bitset.") {}
    }; // class invalid_bitset
} // namespace nstd
