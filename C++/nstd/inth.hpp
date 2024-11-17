#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <iomanip>

using ulong_t = unsigned long;

namespace nstd
{
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
         * @param hex_str A string representing a hexadecimal number (e.g., "0x1A3F").
         * @throws invalid_inth if the provided string is not a valid hexadecimal representation.
         */
        inth(const std::string& hex_str) : value_(0)
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
        inth(ulong_t value) : value_(value) {}

        /**
         * @brief Converts the stored value to a hexadecimal string.
         * @return A string representing the hexadecimal value (e.g., "0x1A3F").
         */
        std::string toHex() const
        {
            std::ostringstream oss;
            oss << "0x" << std::uppercase << std::hex << value_;
            return oss.str();
        }

        /**
         * @brief Retrieves the underlying unsigned long value.
         * @return The unsigned long value.
         */
        ulong_t getValue() const
        {
            return value_;
        }

    private:
        ulong_t value_;

        /**
         * @brief Validates whether a string is a valid hexadecimal representation.
         * 
         * @param hex_str The string to validate.
         * @return true if the string is valid; false otherwise.
         */
        static bool isValid(const std::string& hex_str)
        {
            if (hex_str.length() < 3 || hex_str[0] != '0' || hex_str[1] != 'x')
            {
                return false;
            }
            for (size_t i = 2; i < hex_str.length(); ++i)
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
} // namespace nstd
