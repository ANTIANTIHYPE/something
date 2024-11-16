#pragma once

#include <stdexcept>

namespace nstd
{
    template <class traits = std::char_traits<char>, class Allocator = std::allocator<char>>
    class string
    {
    public:
        // types

        typedef typename Allocator::size_type size_type;
        typedef typename Allocator::difference_type difference_type;
        typedef typename Allocator::pointer pointer;
        typedef typename Allocator::const_pointer const_pointer;
        typedef typename Allocator::reference reference;
        typedef typename Allocator::const_reference const_reference;

        // constructors

        string() : string(Allocator()) {}
        string(const Allocator& a) : alloc_(a) {}
        string(size_type n, char c, const Allocator& a = Allocator()) : alloc_(a)
        {
            assign(n, c);
        }
        string(const string& str) : alloc_(str.alloc_)
        {
            assign(str.data_, str.size_);
        }
        string(const string& str, size_type pos, size_type n = npos) : alloc_(str.alloc_)
        {
            assign(str.data_ + pos, n);
        }
        string(const char* s, size_type n, const Allocator& a = Allocator()) : alloc_(a)
        {
            assign(s, n);
        }
        string(const char* s, const Allocator& a = Allocator()) : alloc_(a) {
            assign(s, traits::length(s));
        }
        string(string&& str) : alloc_(std::move(str.alloc_))
        {
            data_ = str.data_;
            size_ = str.size_;
            str.data_ = nullptr;
            str.size_ = 0;
        }
        string& operator=(string&& str)
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

        // assignment operators

        string& operator=(const string& str)
        {
            if (this != &str)
            {
                alloc_ = str.alloc_;
                assign(str.data_, str.size_);
            }
            return *this;
        }
        string& operator=(const char* s)
        {
            assign(s, traits::length(s));
            return *this;
        }
        string& operator=(char c)
        {
            assign(1, c);
            return *this;
        }

        // iterators

        iterator begin() { return iterator(data_); }
        const_iterator begin() const { return const_iterator(data_); }
        iterator end() { return iterator(data_ + size_); }
        const_iterator end() const { return const_iterator(data_ + size_); }

        // capacity

        size_type size() const { return size_; }
        size_type length() const { return size_; }
        size_type max_size() const { return alloc_.max_size(); }
        void resize(size_type n, char c = char())
        {
            if (n > size_)
            {
                size_type len = size_;
                size_ = n;
                data_ = alloc_.allocate(n);
                traits::copy(data_, data_ + len, data_ + len);
                traits::assign(data_ + len, n - len, c);
            }
            else
            {
                erase(data_ + n, data_ + size_);
            }
        }
        void reserve(size_type n = 0)
        {
            if (n > alloc_.max_size())
            {
                throw std::length_error("reserve");
            }
            if (n > capacity())
            {
                size_type cap = capacity();
                data_ = alloc_.allocate(n);
                traits::copy(data_, data_ + size_, data_);
                alloc_.deallocate(data_, cap);
            }
        }
        size_type capacity() const { return alloc_.capacity(); }
        bool empty() const { return size_ == 0; }

        // element acces

        reference operator[](size_type pos) { return data_[pos]; }
        const_reference operator[](size_type pos) const { return data_[pos]; }
        reference at(size_type pos)
        {
            if (pos >= size_)
            {
                throw std::out_of_range("at");
            }
            return data_[pos];
        }
        const_reference at(size_type pos) const
        {
            if (pos >= size_)
            {
                throw std::out_of_range("at");
            }
            return data_[pos];
        }
        reference front() { return data_[0]; }
        const_reference front() const { return data_[0]; }
        reference back() { return data_[size_ - 1]; }
        const_reference back() const { return data_[size_ - 1]; }

        // modifiers

        void push_back(char c)
        {
            if (size_ == capacity())
            {
                reserve(size_ + 1);
            }
            traits::assign(data_ + size_++, c);
        }
        void pop_back()
        {
            if (empty())
            {
                throw std::out_of_range("pop_back");
            }
            traits::assign(data_ + --size_, 0);
        }
        string& operator+=(const string& str)
        {
            if (str.size_ > 0)
            {
                size_type len = size_;
                size_ += str.size_;
                if (size_ > capacity())
                {
                    reserve(size_);
                }
                traits::copy(data_ + len, data_ + len + str.size_, str.data_);
            }
            return *this;
        }
        string& operator+=(char c)
        {
            push_back(c);
            return *this;
        }
        string& operator+=(const char* s)
        {
            size_type len = traits::length(s);
            size_ += len;
            if (size_ > capacity())
            {
                reserve(size_);
            }
            traits::copy(data_ + size_ - len, data_ + size_, s);
            return *this;
        }
        void swap(string& str)
        {
            std::swap(alloc_, str.alloc_);
            std::swap(data_, str.data_);
            std::swap(size_, str.size_);
        }

        // insert

        iterator insert(iterator p, char c)
        {
            size_type pos = p - begin();
            push_back(c);
            traits::move(data_ + pos, data_ + pos + 1, data_ + pos);
            return iterator(data_ + pos);
        }
        iterator insert(iterator p, size_type n, char c)
        {
            size_type pos = p - begin();
            size_ += n;
            if (size_ > capacity())
            {
                reserve(size_);
            }
            traits::move(data_ + pos, data_ + pos + n, data_ + pos);
            traits::assign(data_ + pos, n, c);
            return iterator(data_ + pos);
        }
        iterator insert(iterator p, const char* s, size_type n)
        {
            size_type pos = p - begin();
            size_ += n;
            if (size_ > capacity())
            {
                reserve(size_);
            }
            traits::move(data_ + pos, data_ + pos + n, data_ + pos);
            traits::copy(data_ + pos, data_ + pos + n, s);
            return iterator(data_ + pos);
        }
        iterator insert(iterator p, const char* s)
        {
            size_type pos = p - begin();
            size_type len = traits::length(s);
            size_ += len;
            if (size_ > capacity())
            {
                reserve(size_);
            }
            traits::move(data_ + pos, data_ + pos + len, data_ + pos);
            traits::copy(data_ + pos, data_ + pos + len, s);
            return iterator(data_ + pos);
        }

        // erase

        iterator erase(iterator p)
        {
            size_type pos = p - begin();
            traits::move(data_ + pos + 1, data_ + size_, data_ + pos);
            --size_;
            return iterator(data_ + pos);
        }
        iterator erase(iterator first, iterator last)
        {
            size_type pos = first - begin();
            size_type len = last - first;
            traits::move(data_ + pos + len, data_ + size_, data_ + pos);
            size_ -= len;
            return iterator(data_ + pos);
        }

        // replace

        string& replace(size_type pos, size_type n, const char* s, size_type slen)
        {
            erase(data_ + pos, data_ + pos + n);
            insert(data_ + pos, s, slen);
            return *this;
        }
        string& replace(size_type pos, size_type n, const char* s)
        {
            erase(data_ + pos, data_ + pos + n);
            insert(data_ + pos, s, traits::length(s));
            return *this;
        }
        string& replace(iterator i1, iterator i2, const char* s, size_type slen)
        {
            erase(i1, i2);
            insert(i1, s, slen);
            return *this;
        }
        string& replace(iterator i1, iterator i2, const char* s)
        {
            erase(i1, i2);
            insert(i1, s, traits::length(s));
            return *this;
        }

        // find

        size_type find(const char* s, size_type pos = 0) const
        {
            size_type len = traits::length(s);
            for (; pos <= size_ - len; ++pos) {
                if (traits::eq(data_[pos], s[0]) && traits::compare(data_ + pos, data + pos + len, s, len) == 0)
                {
                    return pos;
                }
            }
            return npos;
        }
        size_type find(char c, size_type pos = 0) const
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
        size_type find(const string& str, size_type pos = 0) const
        {
            return find(str.data_, pos, str.size_);
        }
        size_type find(const char* s, size_type pos, size_type n) const
        {
            size_type len = n;
            for (; pos <= size_ - len; ++pos)
            {
                if (traits::eq(data_[pos], s[0]) && traits::compare(data_ + pos, data_ + pos + len, s, len) == 0)
                {
                    return pos;
                }
            }
            return npos;
        }

        // substr

        string substr(size_type pos = 0, size_type n = npos) const
        {
            if (pos > size_)
            {
                throw std::out_of_range("substr");
            }
            if (n > size_ - pos)
            {
                n = size_ - pos;
            }
            return string(data_ + pos, n);
        }

        // compare

        int compare(const string& str) const
        {
            size_type len = size_;
            size_type slen = str.size_;
            size_type n = (len < slen) ? len : slen;
            int r = traits::compare(data_, str.data_, n);
            if (r != 0)
            {
                return r;
            }
            if (len < slen)
            {
                return -1;
            }
            if (len > slen)
            {
                return 1;
            }
            return 0;
        }
        int compare(size_type pos, size_type n, const string& str) const
        {
            return substr(pos, n).compare(str);
        }
        int compare(size_type pos, size_type n, const string& str, size_type pos2, size_type n2) const
        {
            return substr(pos, n).compare(str.substr(pos2, n2));
        }
        int compare(const char* s) const
        {
            return compare(string(s));
        }
        int compare(size_type pos, size_type n, const char* s, size_type slen) const
        {
            return substr(pos, n).compare(string(s, slen));
        }
        int compare(size_type pos, size_type n, const char* s) const
        {
            return substr(pos, n).compare(string(s));
        }

    private:
        Allocator alloc_;
        char* data_;
        size_type size_;
        static constexpr size_type npos = -1;
    }; // class string
} // namespace nstd
