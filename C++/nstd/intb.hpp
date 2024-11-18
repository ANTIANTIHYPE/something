#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <compare>

namespace nstd
{
    /**
     * @brief A class representing arbitrary precision integers.
     * 
     * The `intb` class allows for the representation and manipulation of large integers
     * beyond the limits of standard integer types.
     */
    class intb
    {
    public:
        std::vector<int> digits;

        /**
         * @brief Initializes the digits vector to be empty.
         */
        constexpr intb() = default;

        /**
         * @brief Constructs an intb object from a string representation of a number.
         * 
         * @param number A string containing the number, consisting of digits only.
         * @throws std::invalid_argument if the string contains invalid characters.
         */
        constexpr intb(const std::string& number);

        /**
         * @brief Adds two intb objects.
         * 
         * @param other The intb object to add.
         * @return A new intb object representing the sum.
         */
        constexpr intb operator+(const intb& other) const;

        /**
         * @brief Subtracts another intb object from this intb object.
         * 
         * @param other The intb object to subtract.
         * @return A new intb object representing the difference.
         * @throws std::invalid_argument if the result is negative.
         */
        constexpr intb operator-(const intb& other) const;

        /**
         * @brief Multiplies two intb objects.
         * 
         * @param other The intb object to multiply with.
         * @return A new intb object representing the product.
         */
        constexpr intb operator*(const intb& other) const;

        /**
         * @brief Divides this intb object by another intb object.
         * 
         * @param other The intb object to divide by.
         * @return A new intb object representing the quotient.
         * @throws std::invalid_argument if division by zero occurs.
         */
        constexpr intb operator/(const intb& other) const;

        /**
         * @brief Computes the modulus of this intb object by another intb object.
         * 
         * @param other The intb object to divide by.
         * @return A new intb object representing the remainder.
         * @throws std::invalid_argument if division by zero occurs.
         */
        constexpr intb operator%(const intb& other) const;

        /**
         * @brief Compares this intb object with another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if this intb is less than the other, false otherwise.
         */
        constexpr bool operator<(const intb& other) const;

        /**
         * @brief Compares this intb object with another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if this intb is less than or equal to the other, false otherwise.
         */
        constexpr bool operator<=(const intb& other) const;

        /**
         * @brief Compares this intb object with another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if this intb is greater than the other, false otherwise.
         */
        constexpr bool operator>(const intb& other) const;

        /**
         * @brief Compares this intb object with another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if this intb is greater than or equal to the other, false otherwise.
         */
        constexpr bool operator>=(const intb& other) const;

        /**
         * @brief Checks if this intb object is equal to another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if they are equal, false otherwise.
         */
        constexpr bool operator==(const intb& other) const;

        /**
         * @brief Checks if this intb object is not equal to another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if they are not equal, false otherwise.
         */
        constexpr bool operator!=(const intb& other) const;

        /**
         * @brief Provides a three-way comparison between this intb object and another.
         * 
         * @param other The int b object to compare with.
         * @return A strong_ordering value indicating the result of the comparison.
         */
        constexpr std::strong_ordering operator<=>(const intb& other) const;

        /**
         * @brief Negates this intb object.
         * @return A new intb object representing the negation of this intb.
         */
        constexpr intb operator-() const;

        /**
         * @brief Increments this intb object by one (prefix).
         * @return A reference to this intb object after incrementing.
         */
        intb& operator++();

        /**
         * @brief Increments this intb object by one (postfix).
         * 
         * @param unused An integer parameter to distinguish between prefix and postfix.
         * @return A new intb object representing the value before incrementing.
         */
        intb operator++(int);

        /**
         * @brief Decrements this intb object by one (prefix).
         * @return A reference to this intb object after decrementing.
         */
        intb& operator--();

        /**
         * @brief Decrements this intb object by one (postfix).
         * 
         * @param unused An integer parameter to distinguish between prefix and postfix.
         * @return A new intb object representing the value before decrementing.
         */
        intb operator--(int);

        /**
         * @brief Adds another intb object to this intb object (compound assignment).
         * 
         * @param other The intb object to add.
         * @return A reference to this intb object after addition.
         */
        intb& operator+=(const intb& other);

        /**
         * @brief Subtracts another intb object from this intb object (compound assignment).
         * 
         * @param other The intb object to subtract.
         * @return A reference to this intb object after subtraction.
         */
        intb& operator-=(const intb& other);

        /**
         * @brief Multiplies this intb object by another intb object (compound assignment).
         * 
         * @param other The intb object to multiply with.
         * @return A reference to this intb object after multiplication.
         */
        intb& operator*=(const intb& other);

        /**
         * @brief Divides this intb object by another intb object (compound assignment).
         * 
         * @param other The intb object to divide by.
         * @return A reference to this intb object after division.
         * @throws std::invalid_argument if division by zero occurs.
         */
        intb& operator/=(const intb& other);

        /**
         * @brief Computes the modulus of this intb object by another intb object (compound assignment).
         * 
         * @param other The intb object to divide by.
         * @return A reference to this intb object after modulus.
         * @throws std::invalid_argument if division by zero occurs.
         */
        intb& operator%=(const intb& other);

        /**
         * @brief Removes leading zeros from the digits vector.
         */
        constexpr void removeLeadingZeros();

        /**
         * @brief Outputs the intb object as a string.
         * 
         * @param os The output stream.
         * @param intb The intb object to output.
         * @return The output stream.
         */
        constexpr friend std::ostream& operator<<(std::ostream& os, const intb& intb);
    }; // class intb

    ///<summary>
    /// Implementations
    ///</summary>

    // Constructor

    constexpr intb::intb(const std::string& number)
    {
        if (number.empty() || !std::all_of(number.begin(), number.end(), ::isdigit))
        {
            throw std::invalid_argument("Invalid number string");
        }
        for (char digit : number)
        {
            digits.push_back(digit - '0');
        }
        removeLeadingZeros();
    }

    // Operators

    constexpr intb intb::operator+(const intb& other) const
    {
        intb result;
        int carry = 0;
        std::size_t maxLength = std::max(digits.size(), other.digits.size());
        for (std::size_t i = 0; i < maxLength || carry; ++i)
        {
            if (i == digits.size()) result.digits.push_back(0);
            if (i == other.digits.size()) result.digits.push_back(0);
            carry += (i < digits.size() ? digits[i] : 0) + (i < other.digits.size() ? other.digits[i] : 0);
            result.digits[i] = carry % 10;
            carry /= 10;
        }
        return result;
    }

    constexpr intb intb::operator-(const intb& other) const
    {
        if (*this < other)
        {
            throw std::invalid_argument("Negative result in subtraction");
        }

        intb result;
        result.digits.resize(digits.size());

        int borrow = 0;
        for (std::size_t i = 0; i < digits.size(); ++i)
        {
            int sub = digits[i] - borrow;
            if (i < other.digits.size())
            {
                sub -= other.digits[i];
            }

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

    constexpr intb intb::operator*(const intb& other) const
    {
        intb result;
        result.digits.resize(digits.size() + other.digits.size(), 0);

        for (std::size_t i = 0; i < digits.size(); ++i)
        {
            for (std::size_t j = 0; j < other.digits.size(); ++j)
            {
                result.digits[i + j] += digits[i] * other.digits[j];
                if (result.digits[i + j] >= 10)
                {
                    result.digits[i + j + 1] += result.digits[i + j] / 10;
                    result.digits[i + j] %= 10;
                }
            }
        }

        result.removeLeadingZeros();
        return result;
    }

    constexpr intb intb::operator/(const intb& other) const
    {
        if (other == intb("0"))
        {
            throw std::invalid_argument("Division by zero");
        }

        intb result("0");
        intb current("0");
        for (std::size_t i = digits.size(); i > 0; --i)
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

    constexpr intb intb::operator%(const intb& other) const
    {
        if (other == intb("0"))
        {
            throw std::invalid_argument("Division by zero");
        }

        intb remainder("0");
        for (std::size_t i = digits.size(); i > 0; --i)
        {
            remainder.digits.insert(remainder.digits.begin(), digits[i - 1]);
            remainder.removeLeadingZeros();
            while (remainder >= other)
            {
                remainder = remainder - other;
            }
        }

        remainder.removeLeadingZeros();
        return remainder;
    }

    constexpr bool intb::operator<(const intb& other) const
    {
        if (digits.size() != other.digits.size())
        {
            return digits.size() < other.digits.size();
        }
        for (std::size_t i = digits.size(); i-- > 0;)
        {
            if (digits[i] != other.digits[i])
            {
                return digits[i] < other.digits[i];
            }
        }
        return false;
    }

    constexpr bool intb::operator<=(const intb& other) const
    {
        return *this < other || *this == other;
    }

    constexpr bool intb::operator>(const intb& other) const
    {
        return !(*this <= other);
    }

    constexpr bool intb::operator>=(const intb& other) const
    {
        return !(*this < other);
    }

    constexpr bool intb::operator==(const intb& other) const
    {
        return digits == other.digits;
    }

    constexpr bool intb::operator!=(const intb& other) const
    {
        return !(*this == other);
    }

    constexpr std::strong_ordering intb::operator<=>(const intb& other) const
    {
        if (*this < other)
        {
            return std::strong_ordering::less;
        }
        if (*this > other)
        {
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }

    constexpr intb intb::operator-(const intb& other) const
    {
        if (*this < other)
        {
            throw std::invalid_argument("Negative result in subtraction");
        }

        intb result;
        result.digits.resize(digits.size());

        int borrow = 0;
        for (std::size_t i = 0; i < digits.size(); ++i)
        {
            int sub = digits[i] - borrow;
            if (i < other.digits.size())
            {
                sub -= other.digits[i];
            }

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

    constexpr intb& intb::operator++() noexcept
    {
        *this += intb("1");
        return *this;
    }

    constexpr intb intb::operator++(int) noexcept
    {
        intb temp = *this;
        ++(*this);
        return temp;
    }

    constexpr intb& intb::operator--() noexcept
    {
        *this -= intb("1");
        return *this;
    }

    constexpr intb intb::operator--(int) noexcept
    {
        intb temp = *this;
        --(*this);
        return temp;
    }

    constexpr intb& intb::operator+=(const intb& other) noexcept
    {
        *this = *this + other;
        return *this;
    }

    constexpr intb& intb::operator-=(const intb& other) noexcept
    {
        *this = *this - other;
        return *this;
    }

    constexpr intb& intb::operator*=(const intb& other) noexcept
    {
        *this = *this * other;
        return *this;
    }

    constexpr intb& intb::operator/=(const intb& other)
    {
        *this = *this / other;
        return *this;
    }

    constexpr intb& intb::operator%=(const intb& other)
    {
        *this = *this % other;
        return *this;
    }

    constexpr void intb::removeLeadingZeros() noexcept
    {
        while (digits.size() > 1 && digits.back() == 0)
        {
            digits.pop_back();
        }
    }

    constexpr std::ostream& operator<<(std::ostream& os, const intb& intb)
    {
        for (auto it = intb.digits.rbegin(); it != intb.digits.rend(); ++it)
        {
            os << *it;
        }
        return os;
    }
} // namespace nstd
