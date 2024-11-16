#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

namespace nstd
{
    class intb
    {
    public:
        std::vector<int> digits;

        intb(const std::string& number)
        {
            for (char digit : number)
            {
                if (isdigit(digit))
                {
                    digits.push_back(digit - '0');
                }
                else
                {
                    throw std::invalid_argument("Invalid character in number");
                }
            }
            std::reverse(digits.begin(), digits.end());
        }

        intb operator+(const intb& other) const
        {
            intb result("0");
            result.digits.resize(std::max(digits.size(), other.digits.size()) + 1, 0);
            
            int carry = 0;
            for (size_t i = 0; i < result.digits.size(); ++i)
            {
                int sum = carry;
                if (i < digits.size()) sum += digits[i];
                if (i < other.digits.size()) sum += other.digits[i];
                result.digits[i] = sum % 10;
                carry = sum / 10;
            }

            result.removeLeadingZeros();
            return result;
        }

        intb operator-(const intb& other) const
        {
            if (*this < other)
            {
                throw std::invalid_argument("Negative result in subtraction");
            }
            
            intb result("0");
            result.digits.resize(digits.size(), 0);
            
            int borrow = 0;
            for (size_t i = 0; i < digits.size(); ++i)
            {
                int sub = digits[i] - borrow;
                if (i < other.digits.size()) sub -= other.digits[i];
                if (sub < 0)
                {
                    sub += 10;
                    borrow = 1;
                }
                else
                {
                    borrow = 0;
                }
                result.digits[i] = sub;
            }

            result.removeLeadingZeros();
            return result;
        }

        intb operator*(const intb& other) const
        {
            intb result("0");
            result.digits.resize(digits.size() + other.digits.size(), 0);

            for (size_t i = 0; i < digits.size(); ++i)
            {
                for (size_t j = 0; j < other.digits.size(); ++j)
                {
                    result.digits[i + j] += digits[i] * other.digits[j];
                    if (result.digits[i + j] >= 10) {
                        result.digits[i + j + 1] += result.digits[i + j] / 10;
                        result.digits[i + j] %= 10;
                    }
                }
            }

            result.removeLeadingZeros();
            return result;
        }

        intb operator/(const intb& other) const
        {
            if (other == intb("0"))
            {
                throw std::invalid_argument("Division by zero");
            }
            
            intb result("0");
            intb current("0");
            for (size_t i = digits.size(); i > 0; --i)
            {
                current.digits.insert(current.digits.begin(), digits[i - 1]);
                current.removeLeadingZeros();
                int count = 0;
                while (current >= other)
                {
                    current = current - other;
                    count++;
                }
                result.digits.push_back(count);
            }

            std::reverse(result.digits.begin(), result.digits.end());
            result.removeLeadingZeros();
            return result;
        }

        bool operator<(const intb& other) const
        {
            if (digits.size() != other.digits.size())
            {
                return digits.size() < other.digits.size();
            }
            for (size_t i = digits.size(); i-- > 0;)
            {
                if (digits[i] != other.digits[i])
                {
                    return digits[i] < other.digits[i];
                }
            }
            return false;
        }

        bool operator<=(const intb& other) const
        {
            return *this < other || *this == other;
        }

        bool operator>(const intb& other) const
        {
            return !(*this <= other);
        }

        bool operator>=(const intb& other) const
        {
            return !(*this < other);
        }

        bool operator==(const intb& other) const
        {
            return digits == other.digits;
        }

        bool operator!=(const intb& other) const
        {
            return !(*this == other);
        }

        std::strong_ordering operator<=>(const intb& other) const
        {
            if (*this < other) return std::strong_ordering::less;
            if (*this > other) return std::strong_ordering::greater;
            return std::strong_ordering::equal;
        }

        void removeLeadingZeros()
        {
            while (digits.size() > 1 && digits.back() == 0)
            {
                digits.pop_back();
            }
        }

        // Output operator
        friend std::ostream& operator<<(std::ostream& os, const intb& intb)
        {
            for (auto it = intb.digits.rbegin(); it != intb.digits.rend(); ++it)
            {
                os << *it;
            }
            return os;
        }
    };
}
