#pragma once

#include <stdexcept>
#include <memory>
#include <cstring>

namespace nstd
{
    /**
     * @brief A string class that provides dynamic string management.
     * 
     * This class supports various string operations, including construction,
     * assignment, concatenation, and substring extraction. It uses custom
     * allocators to manage memory.
     *
     * @tparam traits Character traits type (default is std::char_traits<char>).
     * @tparam Allocator Allocator type (default is std::allocator<char>).
     */
    template <class traits = std::char_traits<char>, class Allocator = std::allocator<char>>
    class string
    {
    public:
        using size_type = typename Allocator::size_type;
        using difference_type = typename Allocator::difference_type;
        using pointer = typename Allocator::pointer;
        using const_pointer = typename Allocator::const_pointer;
        using reference = typename Allocator::reference;
        using const_reference = typename Allocator::const_reference;

        /**
         * @brief Default constructor.
         */
        constexpr string() : string(Allocator()) {}

        /**
         * @brief Constructs a string with a specified allocator.
         * @param alloc The allocator to use.
         */
        constexpr explicit string(const Allocator& alloc) : alloc_(alloc), data_(nullptr), size_(0) {}

        /**
         * @brief Constructs a string with a specified size and fill character.
         * 
         * @param n The number of characters.
         * @param c The fill character.
         * @param alloc The allocator to use (default is Allocator()).
         */
        constexpr string(size_type n, char c, const Allocator& alloc = Allocator()) : alloc_(alloc)
        {
            assign(n, c);
        }

        /**
         * @brief Copy constructor.
         * @param str The string to copy from.
         */
        constexpr string(const string& str) : alloc_(str.alloc_)
        {
            assign(str.data_, str.size_);
        }

        /**
         * @brief Constructs a substring from another string.
         * 
         * @param str The source string.
         * @param pos The starting position.
         * @param n The number of characters to copy (default is npos).
         */
        constexpr string(const string& str, size_type pos, size_type n = npos) : alloc_(str.alloc_)
        {
            assign(str.data_ + pos, n);
        }

        /**
         * @brief Constructs a string from a C-style string with specified length.
         * 
         * @param s The C-style string.
         * @param n The number of characters to copy.
         * @param alloc The allocator to use (default is Allocator()).
         */
        constexpr string(const char* s, size_type n, const Allocator& alloc = Allocator()) : alloc_(alloc)
        {
            assign(s, n);
        }

        /**
         * @brief Constructs a string from a C-style string.
         * 
         * @param s The C-style string.
         * @param alloc The allocator to use (default is Allocator()).
         */
        constexpr string(const char* s, const Allocator& alloc = Allocator()) : alloc_(alloc)
        {
            assign(s, traits::length(s));
        }

        /**
         * @brief Move constructor.
         * @param str The string to move from.
         */
        constexpr string(string&& str) noexcept : alloc_(std::move(str.alloc_)), data_(str.data_), size_(str.size_)
        {
            str.data_ = nullptr;
            str.size_ = 0;
        }

        /**
         * @brief Move assignment operator.
         * 
         * @param str The string to move from.
         * @return A reference to this string.
         */
        constexpr string& operator=(string&& str) const
        {
            if (this != &str)
            {
                alloc_ = std::move(str.alloc_);
                data_ = str.data_;
                size_ = str.size_;
                str.data_ = nullptr;
                str.size_ = 0;
            }
            return *this;
        }

        /**
         * @brief Copy assignment operator.
         * 
         * @param str The string to copy from.
         * @return A reference to this string.
         */
        constexpr string& operator=(const string& str) noexcept
        {
            if (this != &str)
            {
                alloc_ = str.alloc_;
                assign(str.data_, str .size_);
            }
            return *this;
        }

        /**
         * @brief Assignment operator for C-style strings.
         * 
         * @param s The C-style string to assign.
         * @return A reference to this string.
         */
        constexpr string& operator=(const char* s) noexcept
        {
            assign(s, traits::length(s));
            return *this;
        }

        /**
         * @brief Assignment operator for a single character.
         * 
         * @param c The character to assign.
         * @return A reference to this string.
         */
        constexpr string& operator=(char c)
        {
            assign(1, c);
            return *this;
        }

        using iterator = char*;
        using const_iterator = const char*;

        /**
         * @brief Returns an iterator to the beginning of the string.
         * @return An iterator to the first character.
         */
        constexpr iterator begin() { return data_; }

        /**
         * @brief Returns a const iterator to the beginning of the string.
         * @return A const iterator to the first character.
         */
        constexpr const_iterator begin() const { return data_; }

        /**
         * @brief Returns an iterator to the end of the string.
         * @return An iterator to one past the last character.
         */
        constexpr iterator end() { return data_ + size_; }

        /**
         * @brief Returns a const iterator to the end of the string.
         * @return A const iterator to one past the last character.
         */
        constexpr const_iterator end() const { return data_ + size_; }

        /**
         * @brief Returns the current size of the string.
         * @return The number of characters in the string.
         */
        constexpr size_type size() { return size_; }

        /**
         * @brief Returns the length of the string.
         * @return The number of characters in the string.
         */
        constexpr size_type length() { return size_; }

        /**
         * @brief Returns the maximum size of the string.
         * @return The maximum number of characters the string can hold.
         */
        constexpr size_type max_size() const { return alloc_.max_size(); }

        /**
         * @brief Resizes the string to the specified size.
         * 
         * @param n The new size.
         * @param c The fill character (default is char()).
         */
        constexpr void resize(size_type n, char c = char())
        {
            if (n > size_)
            {
                reserve(n);
                traits::assign(data_ + size_, n - size_, c);
            }
            size_ = n;
        }

        /**
         * @brief Reserves space for at least the specified number of characters.
         * @param n The number of characters to reserve space for (default is 0).
         */
        constexpr void reserve(size_type n)
        {
            if (n > alloc_.max_size())
            {
                throw std::length_error("reserve");
            }
            if (n > capacity())
            {
                pointer new_data = alloc_.allocate(n);
                if (data_)
                {
                    traits::copy(new_data, data_, size_);
                    alloc_.deallocate(data_, capacity());
                }
                data_ = new_data;
            }
        }

        /**
         * @brief Returns the current capacity of the string.
         * @return The number of characters that can be held without reallocating.
         */
        constexpr size_type capacity() const { return alloc_.max_size(); }

        /**
         * @brief Checks if the string is empty.
         * @return True if the string is empty, false otherwise.
         */
        constexpr bool empty() const { return size_ == 0; }

        /**
         * @brief Accesses the character at the specified position.
         * 
         * @param pos The position of the character.
         * @return A reference to the character at the specified position.
         */
        constexpr reference operator[](size_type pos) { return data_[pos]; }

        /**
         * @brief Accesses the character at the specified position (const).
         * 
         * @param pos The position of the character.
         * @return A const reference to the character at the specified position.
         */
        constexpr const_reference operator[](size_type pos) const { return data_[pos]; }

        /**
         * @brief Accesses the character at the specified position with bounds checking.
         * 
         * @param pos The position of the character.
         * @return A reference to the character at the specified position.
         * @throws std::out_of_range if the position is out of bounds.
         */
        constexpr reference at(size_type pos)
        {
            if (pos >= size_)
            {
                throw std::out_of_range("at");
            }
            return data_[pos];
        }

        /**
         * @brief Accesses the character at the specified position with bounds checking (const).
         * 
         * @param pos The position of the character.
         * @return A const reference to the character at the specified position.
         * @throws std::out_of_range if the position is out of bounds.
         */
        constexpr const_reference at(size_type pos) const
        {
            if (pos >= size_)
            {
                throw std::out_of_range("at");
            }
            return data_[pos];
        }

        /**
         * @brief Returns a reference to the first character.
         * @return A reference to the first character.
         */
        constexpr reference front() { return data_[0]; }

        /**
         * @brief Returns a const reference to the first character.
         * @return A const reference to the first character.
         */
        constexpr const_reference front() const { return data_[0]; }

        /**
         * @brief Returns a reference to the last character.
         * @return A reference to the last character.
         */
        constexpr reference back() { return data_[size_ - 1]; }

        /**
         * @brief Returns a const reference to the last character.
         * @return A const reference to the last character.
         */
        constexpr const_reference back() const { return data_[size_ - 1]; }

        /**
         * @brief Adds a character to the end of the string.
         * @param c The character to add.
         */
        constexpr void push_back(char c)
        {
            if (size_ == capacity())
            {
                reserve(size_ + 1);
            }
            data_[size_++] = c;
        }

        /**
         * @brief Removes the last character from the string.
         * @throws std::out_of_range if the string is empty.
         */
        constexpr void pop_back()
        {
            if (empty())
            {
                throw std::out_of_range("pop_back");
            }
            --size_;
        }

        /**
         * @brief Swaps the contents of this string with another string.
         * @param str The string to swap with.
         */
        constexpr void swap(string& str)
        {
            std::swap(alloc_, str.alloc_);
            std::swap(data_, str.data_);
            std::swap(size_, str.size_);
        }

        /**
         * @brief Concatenates another string to this string.
         * 
         * @param str The string to concatenate.
         * @return A reference to this string.
         */
        constexpr string& operator+=(const string& str)
        {
            size_type new_size = size_ + str.size_;
            reserve(new_size);
            traits::copy(data_ + size_, str.data_, str.size_);
            size_ = new_size;
            return *this;
        }

        /**
         * @brief Concatenates a character to this string.
         * 
         * @param c The character to concatenate.
         * @return A reference to this string.
         */
        constexpr string& operator+=(char c)
        {
            push_back(c);
            return *this;
        }

        /**
         * @brief Concatenates a C-style string to this string.
         * 
         * @param s The C-style string to concatenate.
         * @return A reference to this string.
         */
        constexpr string& operator+=(const char* s)
        {
            size_type len = traits::length(s);
            size_type new_size = size_ + len;
            reserve(new_size);
            traits::copy(data_ + size_, s, len);
            size_ = new_size;
            return *this;
        }

        /**
         * @brief Inserts a character at the specified position.
         * 
         * @param p The position to insert at.
         * @param c The character to insert.
         * @return An iterator to the inserted character.
         */
        constexpr iterator insert(iterator p, char c)
        {
            size_type pos = p - begin();
            push_back(c);
            traits::move(data_ + pos, data_ + pos + 1, data_ + pos);
            return data_ + pos;
        }

        /**
         * @brief Inserts multiple copies of a character at the specified position.
         * 
         * @param p The position to insert at.
         * @param n The number of copies to insert.
         * @param c The character to insert.
         * @return An iterator to the first inserted character.
         */
        constexpr iterator insert(iterator p, size_type n, char c)
        {
            size_type pos = p - begin();
            reserve(size_ + n);
            traits::move(data_ + pos + n, data_ + pos, size_ - pos);
            traits::assign(data_ + pos, n, c);
            size_ += n;
            return data_ + pos;
        }

        /**
         * @brief Inserts a C-style string at the specified position.
         * 
         * @param p The position to insert at.
         * @param s The C-style string to insert.
         * @param n The number of characters to insert.
         * @return An iterator to the first inserted character.
         */
        constexpr iterator insert(iterator p, const char* s, size_type n)
        {
            size_type pos = p - begin();
            reserve(size_ + n);
            traits::move(data_ + pos + n, data_ + pos, size_ - pos);
            traits::copy(data_ + pos, s, n);
            size_ += n;
            return data_ + pos;
        }

        /**
         * @brief Erases a character at the specified position.
         * 
         * @param p The position of the character to erase.
         * @return An iterator to the next character after the erased character.
         */
        constexpr iterator erase(iterator p)
        {
            size_type pos = p - begin();
            traits::move(data_ + pos + 1, data_ + size_, data_ + pos);
            --size_;
            return data_ + pos;
        }

        /**
         * @brief Erases a range of characters.
         * 
         * @param first The beginning of the range to erase.
         * @param last The end of the range to erase.
         * @return An iterator to the first character after the erased range.
         */
        constexpr iterator erase(iterator first, iterator last)
        {
            size_type pos = first - begin();
            size_type len = last - first;
            traits::move(data_ + pos + len, data_ + size_, data_ + pos);
            size_ -= len;
            return data_ + pos;
        }

        /**
         * @brief Returns a substring starting from the specified position.
         * 
         * @param pos The starting position.
         * @param n The number of characters to include (default is npos).
         * @return A new string containing the substring.
         * @throws std::out_of_range if the position is out of bounds.
         */
        constexpr string substr(size_type pos = 0, size_type n = npos) const
        {
            if (pos > size_)
            {
                throw std::out_of_range("substr");
            }
            if (n > size_ - pos)
            {
                n = size_ - pos;
            }
            return string(data_ + pos, n, alloc_);
        }

        /**
         * @brief Finds the first occurrence of a C-style string.
         * 
         * @param s The C-style string to find.
         * @param pos The position to start searching from (default is 0).
         * @return The position of the first occurrence, or npos if not found.
         */
        constexpr size_type find(const char* s, size_type pos = 0) const
        {
            size_type len = traits::length(s);
            for (; pos <= size_ - len; ++pos)
            {
                if (traits::compare(data_ + pos, s, len) == 0)
                {
                    return pos;
                }
            }
            return npos;
        }

        /**
         * @brief Finds the first occurrence of a character.
         * 
         * @param c The character to find.
         * @param pos The position to start searching from (default is 0).
         * @return The position of the first occurrence, or npos if not found.
         */
        constexpr size_type find(char c, size_type pos = 0) const
        {
            for (; pos < size_; ++pos)
            {
                if (traits::eq(data_[pos], c))
                {
                    return pos;
                }
            }
            return npos;
        }

        /**
         * @brief Compares this string with another string.
         * 
         * @param str The string to compare with.
         * @return A negative value if this string is less than str, 
         *         a positive value if this string is greater than str, 
         *         and zero if they are equal.
         */
        constexpr int compare(const string& str) const
        {
            size_type len = size_;
            size_type slen = str.size_;
            size_type n = (len < slen) ? len : slen;
            int r = traits::compare(data_, str.data_, n);
            if (r != 0)
            {
                return r;
            }
            return (len < slen) ? -1 : (len > slen) ? 1 : 0;
        }

        /**
         * @brief Compares a substring of this string with another string.
         * 
         * @param pos The starting position in this string.
         * @param n The number of characters to compare.
         * @param str The string to compare with.
         * @return A negative value if this substring is less than str, 
         *         a positive value if this substring is greater than str, 
         *         and zero if they are equal.
         */
        constexpr int compare(size_type pos, size_type n, const string& str) const
        {
            return substr(pos, n).compare(str);
        }

        /**
         * @brief Compares a substring of this string with a substring of another string.
         * 
         * @param pos The starting position in this string.
         * @param n The number of characters to compare.
         * @param str The string to compare with.
         * @param pos2 The starting position in the other string.
         * @param n2 The number of characters to compare in the other string.
         * @return A negative value if this substring is less than the other substring, 
         *         a positive value if this substring is greater than the other substring, 
         *         and zero if they are equal.
         */
        constexpr int compare(size_type pos, size_type n, const string& str, size_type pos2, size_type n2) const
        {
            return substr(pos, n).compare(str.substr(pos2, n2));
        }

        /**
         * @brief Compares this string with a C-style string.
         * 
         * @param s The C-style string to compare with.
         * @return A negative value if this string is less than s, 
         *         a positive value if this string is greater than s, 
         *         and zero if they are equal.
         */
        constexpr int compare(const char* s) const
        {
            return compare(string(s));
        }

        /**
         * @brief Compares a substring of this string with a C-style string.
         * 
         * @param pos The starting position in this string.
         * @param n The number of characters to compare.
         * @param s The C-style string to compare with.
         * @param slen The number of characters in the C-style string.
         * @return A negative value if this substring is less than s, 
         *         a positive value if this substring is greater than s, 
         *         and zero if they are equal.
         */
        constexpr int compare(size_type pos, size_type n, const char* s, size_type slen) const
        {
            return substr(pos, n).compare(string(s, slen));
        }

        /**
         * @brief Compares a substring of this string with a C-style string.
         * 
         * @param pos The starting position in this string.
         * @param n The number of characters to compare.
         * @param s The C-style string to compare with.
         * @return A negative value if this substring is less than s, 
         *         a positive value if this substring is greater than s, 
         *         and zero if they are equal.
         */
        constexpr int compare(size_type pos, size_type n, const char* s) const
        {
            return substr(pos, n).compare(string(s));
        }

    private:
        Allocator alloc_;
        char* data_ = nullptr;
        size_type size_ = 0;
        static constexpr size_type npos = static_cast<size_type>(-1);

        /**
         * @brief Assigns a C-style string to this string.
         * 
         * @param s The C-style string.
         * @param n The number of characters to assign.
         */
        constexpr void assign(const char* s, size_type n)
        {
            reserve(n);
            traits::copy(data_, s, n);
            size_ = n;
        }

        /**
         * @brief Assigns a character to this string multiple times.
         * 
         * @param n The number of times to assign the character.
         * @param c The character to assign.
         */
        constexpr void assign(size_type n, char c)
        {
            reserve(n);
            traits::assign(data_, n, c);
            size_ = n;
        }
    }; // class string
} // namespace nstd
