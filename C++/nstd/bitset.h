#pragma once

#include <stdexcept>
#include <cstdint>
#include <vector>

namespace nstd {
    class bitset {
    public:
        bitset(const std::string& bit_str) : size_(bit_str.size()), data_(size_ / 8 + (size_ % 8 == 0 ? 0 : 1), 0)
        {
            if (bit_str.empty())
            {
                throw std::runtime_error("Empty bitset");
            }
            for (size_t i = 0; i < bit_str.size(); ++i)
            {
                if (bit_str[i] != '0' && bit_str[i] != '1')
                {
                    throw std::runtime_error("Invalid bitset string");
                }
                data_[i / 8] |= (bit_str[i] - '0') << (i % 8);
            }
        }

        bitset(size_t size) : size_(size), data_(size / 8 + (size % 8 == 0 ? 0 : 1), 0) {}

        bool getBit(size_t pos) const
        {
            if (pos >= size_)
            {
                throw std::runtime_error("Bit position out of range");
            }
            return (data_[pos / 8] >> (pos % 8)) & 1;
        }

        void setBit(size_t pos, bool value)
        {
            if (pos >= size_)
            {
                throw std::runtime_error("Bit position out of range");
            }
            if (value)
            {
                data_[pos / 8] |= 1 << (pos % 8);
            }
            else
            {
                data_[pos / 8] &= ~(1 << (pos % 8));
            }
        }

        std::string toString() const
        {
            std::string result;
            for (size_t i = 0; i < size_; ++i)
            {
                result += (data_[i / 8] >> (i % 8)) & 1 ? '1' : '0';
            }
            return result;
        }

        std::vector<uint32_t> getData() const
        {
            return data_;
        }

        void resize(size_t newSize)
        {
            if (newSize < size_)
            {
                throw std::runtime_error("Cannot shrink bitset"); // nvm
            }
            size_ = newSize;
            data_.resize(newSize / 8 + (newSize % 8 == 0 ? 0 : 1), 0);
        }

    private:
        size_t size_;
        std::vector<uint32_t> data_;
    }; // class bitset

    class invalid_bitset : public std::runtime_error {
    public:
        invalid_bitset(const std::string& what) : std::runtime_error(what + " is not a valid bitset.") {}
    }; // class invalid_bitset
} // namespace nstd
