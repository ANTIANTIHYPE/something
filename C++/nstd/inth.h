#pragma once

#include <string>
#include <sstream>
#include <stdexcept>

namespace nstd {
    class inth {
    public:
        inth(const std::string& hex_str) : value_(0) {
            if (!isValid(hex_str)) {
                throw invalid_inth(hex_str);
            }

            value_ = std::stoul(hex_str, nullptr, 16);
        }

        inth(unsigned long value) : value_(value) {}

        std::string toHex() const {
            std::ostringstream oss;
            oss << "0x" << std::uppercase << std::hex << value_;
            return oss.str();
        }

        unsigned long getValue() const {
            return value_;
        }

    private:
        unsigned long value_;

        static bool isValid(const std::string& hex_str) {
            if (hex_str.empty() || hex_str.length() < 3 || hex_str[0] != '0' || hex_str[1] != 'x') {
                return false;
            }
            for (size_t i = 2; i < hex_str.length(); ++i) {
                char c = hex_str[i];
                if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))) {
                    return false;
                }
            }
            return true;
        }
    };

    class invalid_inth : public std::runtime_error {
    public:
        invalid_inth(const std::string& what) : std::runtime_error(what + " is not a valid inth.") {}
    };
}