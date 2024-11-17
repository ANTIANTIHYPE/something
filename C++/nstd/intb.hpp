#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace nstd
{
    /**
     * @brief A class representing arbitrary precision integers.
     * 
     * The `intb` class allows for the representation and manipulation of large integers
     * beyond the limits of standard integer types. It supports basic arithmetic operations
     * such as addition, subtraction, multiplication, and division, as well as comparison
     * operators.
     */
    class intb
    {
    public:
        std::vector<int> digits; ///< Vector to store the digits of the integer.

        /**
         * @brief Constructs an intb object from a string representation of a number.
         * 
         * @param number A string containing the number, consisting of digits only.
         * @throws std::invalid_argument if the string contains invalid characters.
         */
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

        /**
         * @brief Adds two intb objects.
         * 
         * @param other The intb object to add.
         * @return A new intb object representing the sum.
         */
        intb operator+(const intb& other) const
        {
            intb result("0");
            result.digits.resize(std::max(digits.size(), other.digits.size()) + 1, 0);

            int carry = 0;
            for (std::size_t i = 0; i < result.digits.size(); ++i)
            {
                int sum = carry;
                if (i < digits.size()) 
                {
                    sum += digits[i];
                }
                if (i < other.digits.size())
                {
                    sum += other.digits[i];
                }
                result.digits[i] = sum % 10;
                carry = sum / 10;
            }

            result.removeLeadingZeros();
            return result;
        }

        /**
         * @brief Subtracts another intb object from this intb object.
         * 
         * @param other The intb object to subtract.
         * @return A new intb object representing the difference.
         * @throws std::invalid_argument if the result is negative.
         */
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

        /**
         * @brief Multiplies two intb objects.
         * 
         * @param other The intb object to multiply with.
         * @return A new intb object representing the product.
         */
        intb operator*(const intb& other) const
        {
            intb result("0");
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

        /**
         * @brief Divides this intb object by another intb object.
         * 
         * @param other The intb object to divide by.
         * @return A new intb object representing the quotient.
         * @throws std::invalid_argument if division by zero occurs.
         */
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

        /**
         * @brief Compares this intb object with another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if this intb is less than the other, false otherwise.
         */
        bool operator<(const intb& other) const
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

        /**
         * @brief Compares this intb object with another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if this intb is less than or equal to the other, false otherwise.
         */
        bool operator<=(const intb& other) const
        {
            return *this < other || *this == other;
        }

        /**
         * @brief Compares this intb object with another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if this intb is greater than the other, false otherwise.
         */
        bool operator>(const intb& other) const
        {
            return !(*this <= other);
        }

        /**
         * @brief Compares this intb object with another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if this intb is greater than or equal to the other, false otherwise.
         */
        bool operator>=(const intb& other) const
        {
            return !(*this < other);
        }

        /**
         * @brief Checks if this intb object is equal to another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if they are equal, false otherwise.
         */
        bool operator==(const intb& other) const
        {
            return digits == other.digits;
        }

        /**
         * @brief Checks if this intb object is not equal to another intb object.
         * 
         * @param other The intb object to compare with.
         * @return true if they are not equal, false otherwise.
         */
        bool operator!=(const intb& other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Provides a three-way comparison between this intb object and another.
         * 
         * @param other The intb object to compare with.
         * @return A strong_ordering value indicating the result of the comparison.
         */
        std::strong_ordering operator<=>(const intb& other) const
        {
            if (*this < other) return std::strong_ordering::less;
            if (*this > other) return std::strong_ordering::greater;
            return std::strong_ordering::equal;
        }

        /**
         * @brief Removes leading zeros from the digits vector.
         */
        void removeLeadingZeros()
        {
            while (digits.size() > 1 && digits.back() == 0)
            {
                digits.pop_back();
            }
        }

        /**
         * @brief Outputs the intb object as a string.
         * 
         * @param os The output stream.
         * @param intb The intb object to output.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const intb& intb)
        {
            for (auto it = intb.digits.rbegin(); it != intb.digits.rend(); ++it)
            {
                os << *it;
            }
            return os;
        }
    }; // class intb
} // namespace nstd
