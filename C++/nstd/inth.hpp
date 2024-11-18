#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <iomanip>

using ulong_t = unsigned long;

namespace nstd
{
    /**
     * @brief Exception thrown for invalid hexadecimal integer operations.
     * 
     * The `invalid_inth` class extends std::runtime_error to indicate that a provided
     * string does not conform to valid hexadecimal integer rules.
     */
    class invalid_inth : public std::runtime_error
    {
    public:
        /**
         * @brief Constructs an invalid_inth exception with a message.
         * @param what A description of the invalid operation or string.
         */
        invalid_inth(const std::string& what) : std::runtime_error(what + " is not a valid hexadecimal int.") {}
    }; // class invalid_inth

    /**
     * @brief A class representing a hexadecimal integer.
     * 
     * The `inth` class allows for the representation and manipulation of hexadecimal integers.
     * It supports conversion from hexadecimal strings and provides methods to retrieve the
     * underlying value and convert it back to a hexadecimal string.
     */
    class inth
    {
    public:
        /**
         * @brief Constructs an inth object from a hexadecimal string.
         * 
         * @param hex_str A string representing a hexadecimal number (e.g. "0x1A3F").
         * @throws invalid_inth if the provided string is not a valid hexadecimal representation.
         */
        constexpr inth(const std::string& hex_str) : value_(0)
        {
            if (!isValid(hex_str))
            {
                throw invalid_inth(hex_str);
            }
            value_ = std::stoul(hex_str, nullptr, 16);
        }

        /**
         * @brief Constructs an inth object from an unsigned long value.
         * @param value The unsigned long value to initialize the inth object.
         */
        constexpr inth(ulong_t value) : value_(value) {}

        /**
         * @brief Converts the stored value to a hexadecimal string.
         * @return A string representing the hexadecimal value (e.g. "0x1A3F").
         */
        constexpr std::string toHex() const
        {
            std::ostringstream oss;
            oss << "0x" << std::uppercase << std::hex << value_;
            return oss.str();
        }

        /**
         * @brief Retrieves the underlying unsigned long value.
         * @return The unsigned long value.
         */
        constexpr ulong_t getValue() const
        {
            return value_;
        }

        // Operators

        /**
         * @brief Adds two inth objects.
         * @param other The inth object to add.
         * @return A new inth object representing the sum.
         */
        constexpr inth operator+(const inth& other) const
        {
            return inth(value_ + other.value_);
        }

        /**
         * @brief Subtracts another inth object from this inth object.
         * 
         * @param other The inth object to subtract.
         * @return A new inth object representing the difference.
         */
        constexpr inth operator-(const inth& other) const
        {
            return inth(value_ - other.value_);
        }

        /**
         * @brief Multiplies two inth objects.
         * 
         * @param other The inth object to multiply with.
         * @return A new inth object representing the product.
         */
        constexpr inth operator*(const inth& other) const
        {
            return inth(value_ * other.value_);
        }

        /**
         * @brief Divides this inth object by another inth object.
         * 
         * @param other The inth object to divide by.
         * @return A new inth object representing the quotient.
         * @throws std::invalid_argument if division by zero occurs.
         */
        constexpr inth operator/(const inth& other) const
        {
            if (other.value_ == 0)
            {
                throw std::invalid_argument("Division by zero.");
            }
            return inth(value_ / other.value_);
        }

        /**
         * @brief Computes the modulus of this inth object by another inth object.
         * 
         * @param other The inth object to divide by.
         * @return A new inth object representing the remainder.
         * @throws std::invalid_argument if division by zero occurs.
         */
        constexpr inth operator%(const inth& other) const
        {
            if (other.value_ == 0)
            {
                throw std::invalid_argument("Division by zero.");
            }
            return inth(value_ % other.value_);
        }

        // // Compound Assignment Operators
        /**
         * @brief Adds another inth object to this inth object.
         * 
         * @param other The inth object to add.
         * @return A reference to this inth object after addition.
         */
        constexpr inth& operator+=(const inth& other) noexcept
        {
            value_ += other.value_;
            return *this;
        }

        /**
         * @brief Subtracts another inth object from this inth object.
         * 
         * @param other The inth object to subtract.
         * @return A reference to this inth object after subtraction.
         */
        constexpr inth& operator-=(const inth& other) noexcept
        {
            value_ -= other.value_;
            return *this;
        }

        /**
         * @brief Multiplies this inth object by another inth object.
         * 
         * @param other The inth object to multiply with.
         * @return A reference to this inth object after multiplication.
         */
        constexpr inth& operator*=(const inth& other) noexcept
        {
            value_ *= other.value_;
            return *this;
        }

        /**
         * @brief Divides this inth object by another inth object.
         * 
         * @param other The inth object to divide by.
         * @return A reference to this inth object after division.
         * @throws std::invalid_argument if division by zero occurs.
         */
        constexpr inth& operator/=(const inth& other) noexcept
        {
            if (other.value_ == 0)
            {
                throw std::invalid_argument("Division by zero.");
            }
            value_ /= other.value_;
            return *this;
        }

        /**
         * @brief Computes the modulus of this inth object by another inth object.
         * 
         * @param other The inth object to divide by.
         * @return A reference to this inth object after modulus.
         * @throws std::invalid_argument if division by zero occurs.
         */
        constexpr inth& operator%=(const inth& other) noexcept
        {
            if (other.value_ == 0)
            {
                throw std::invalid_argument("Division by zero.");
            }
            value_ %= other.value_;
            return *this;
        }

        /**
         * @brief Compares this inth object with another inth object for equality.
         * 
         * @param other The inth object to compare with.
         * @return true if both objects are equal; false otherwise.
         */
        constexpr bool operator==(const inth& other) const
        {
            return value_ == other.value_;
        }

        /**
         * @brief Compares this inth object with another inth object for inequality.
         * 
         * @param other The inth object to compare with.
         * @return true if both objects are not equal; false otherwise.
         */
        constexpr bool operator!=(const inth& other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Compares this inth object with another inth object for less than.
         * 
         * @param other The inth object to compare with.
         * @return true if this object is less than the other; false otherwise.
         */
        constexpr bool operator<(const inth& other) const
        {
            return value_ < other.value_;
        }

        /**
         * @brief Compares this inth object with another inth object for less than or equal to.
         * 
         * @param other The inth object to compare with.
         * @return true if this object is less than or equal to the other; false otherwise.
         */
        constexpr bool operator<=(const inth& other) const
        {
            return value_ <= other.value_;
        }

        /**
         * @brief Compares this inth object with another inth object for greater than.
         * 
         * @param other The inth object to compare with.
         * @return true if this object is greater than the other; false otherwise.
         */
        constexpr bool operator>(const inth& other) const
        {
            return value_ > other.value_;
        }

        /**
         * @brief Compares this inth object with another inth object for greater than or equal to.
         * 
         * @param other The inth object to compare with.
         * @return true if this object is greater than or equal to the other; false otherwise.
         */
        constexpr bool operator>=(const inth& other) const
        {
            return value_ >= other.value_;
        }

        /**
         * @brief Stream insertion operator for inth objects.
         * 
         * @param os The output stream.
         * @param obj The inth object to insert into the stream.
         * @return A reference to the output stream.
         */
        constexpr friend std::ostream& operator<<(std::ostream& os, const inth& obj) noexcept
        {
            os << obj.toHex();
            return os;
        }

    private:
        ulong_t value_;

        /**
         * @brief Validates whether a string is a valid hexadecimal representation.
         * 
         * @param hex_str The string to validate.
         * @return true if the string is valid; false otherwise.
         */
        constexpr static bool isValid(const std::string& hex_str) noexcept
        {
            if (hex_str.length() < 3 || hex_str[0] != '0' || hex_str[1] != 'x')
            {
                return false;
            }
            for (std::size_t i = 2; i < hex_str.length(); ++i)
            {
                char c = hex_str[i];
                if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')))
                {
                    return false;
                }
            }
            return true;
        }
    }; // class inth
} // namespace nstd
