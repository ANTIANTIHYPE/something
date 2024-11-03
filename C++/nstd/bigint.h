#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

namespace nstd {
    class BigInt {
    public:
        std::vector<int> digits;

        BigInt(const std::string& number) {
            for (char digit : number) {
                if (isdigit(digit)) {
                    digits.push_back(digit - '0');
                } else {
                    throw std::invalid_argument("Invalid character in number");
                }
            }
            std::reverse(digits.begin(), digits.end());
        }

        BigInt operator+(const BigInt& other) const {
            BigInt result("0");
            result.digits.resize(std::max(digits.size(), other.digits.size()) + 1, 0);
            
            int carry = 0;
            for (size_t i = 0; i < result.digits.size(); ++i) {
                int sum = carry;
                if (i < digits.size()) sum += digits[i];
                if (i < other.digits.size()) sum += other.digits[i];
                result.digits[i] = sum % 10;
                carry = sum / 10;
            }

            result.removeLeadingZeros();
            return result;
        }

        BigInt operator-(const BigInt& other) const {
            if (*this < other) {
                throw std::invalid_argument("Negative result in subtraction");
            }
            
            BigInt result("0");
            result.digits.resize(digits.size(), 0);
            
            int borrow = 0;
            for (size_t i = 0; i < digits.size(); ++i) {
                int sub = digits[i] - borrow;
                if (i < other.digits.size()) sub -= other.digits[i];
                if (sub < 0) {
                    sub += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                result.digits[i] = sub;
            }

            result.removeLeadingZeros();
            return result;
        }

        BigInt operator*(const BigInt& other) const {
            BigInt result("0");
            result.digits.resize(digits.size() + other.digits.size(), 0);

            for (size_t i = 0; i < digits.size(); ++i) {
                for (size_t j = 0; j < other.digits.size(); ++j) {
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

        BigInt operator/(const BigInt& other) const {
            if (other == BigInt("0")) {
                throw std::invalid_argument("Division by zero");
            }
            
            BigInt result("0");
            BigInt current("0");
            for (size_t i = digits.size(); i > 0; --i) {
                current.digits.insert(current.digits.begin(), digits[i - 1]);
                current.removeLeadingZeros();
                int count = 0;
                while (current >= other) {
                    current = current - other;
                    count++;
                }
                result.digits.push_back(count);
            }

            std::reverse(result.digits.begin(), result.digits.end());
            result.removeLeadingZeros();
            return result;
        }

        bool operator<(const BigInt& other) const {
            if (digits.size() != other.digits.size()) {
                return digits.size() < other.digits.size();
            }
            for (size_t i = digits.size(); i-- > 0;) {
                if (digits[i] != other.digits[i]) {
                    return digits[i] < other.digits[i];
                }
            }
            return false;
        }

        bool operator<=(const BigInt& other) const {
            return *this < other || *this == other;
        }

        bool operator>(const BigInt& other) const {
            return !(*this <= other);
        }

        bool operator>=(const BigInt& other) const {
            return !(*this < other);
        }

        bool operator==(const BigInt& other) const {
            return digits == other.digits;
        }

        bool operator!=(const BigInt& other) const {
            return !(*this == other);
        }

        auto operator<=>(const BigInt& other) const {
            if (*this < other) return std::strong_ordering::less;
            if (*this > other) return std::strong_ordering::greater;
            return std::strong_ordering::equal;
        }

        void removeLeadingZeros() {
            while (digits.size() > 1 && digits.back() == 0) {
                digits.pop_back();
            }
        }

        // Output operator
        friend std::ostream& operator<<(std::ostream& os, const BigInt& bigInt) {
            for (auto it = bigInt.digits.rbegin(); it != bigInt.digits.rend(); ++it) {
                os << *it;
            }
            return os;
        }
    };
}
/*
EXAMPLE:


int main() {
    BigInt a("123456789123456789123456789");
    BigInt b("987654321987654321987654321");
    BigInt sum = a + b;
    BigInt difference = a - b;
    BigInt product = a * b;
    BigInt quotient = a / b;

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Difference: " << difference << std::endl;
    std::cout << "Product: " << product << std::endl;
    std::cout << "Quotient: " << quotient << std::endl;

    std::cout << "a == b: " << (a == b) << std::endl;
    std::cout << "a != b: " << (a != b) << std::endl;
    std::cout << "a < b: " << (a < b) << std::endl;
    std::cout << "a <= b: " << (a <= b) << std::endl;
    std::cout << "a > b: " << (a > b) << std::endl;
    std::cout << "a >= b: " << (a >= b) << std::endl;

    return 0;
}
*/