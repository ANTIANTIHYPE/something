#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <random>

namespace nstd
{
    /**
     * @brief A class representing a quantum array of complex states.
     * 
     * The `QuantumArray` class allows for the representation and manipulation of quantum states
     * using complex numbers.
     */
    class QuantumArray
    {
    public:
        /**
         * @brief Constructs a QuantumArray with a given size.
         * @param n The size of the quantum array.
         */
        constexpr QuantumArray(int n) : size(n)
        {
            if (n <= 0) {
                throw std::invalid_argument("Size must be positive");
            }
            states.resize(n, {0, 0});
        }

        /**
         * @brief Sets the state at a specific index.
         * 
         * @param index The index of the state to set.
         * @param state The complex state to set.
         * @throws std::out_of_range if the index is out of bounds.
         */
        constexpr void set(int index, const std::complex<double>& state)
        {
            if (index < 0 || index >= size)
            {
                throw std::out_of_range("Index out of range");
            }
            states[index] = state;
        }

        /**
         * @brief Gets the state at a specific index.
         * 
         * @param index The index of the state to get.
         * @return The complex state at the specified index.
         * @throws std::out_of_range if the index is out of bounds.
         */
        constexpr std::complex<double> get(int index) const
        {
            if (index < 0 || index >= size)
            {
                throw std::out_of_range("Index out of range");
            }
            return states[index];
        }

        /**
         * @brief Measures the quantum state and outputs the result.
         * 
         * The measurement is probabilistic based on the square of the amplitude of each state.
         * @throws std::runtime_error if the norm of the states is zero.
         */
        constexpr void measure()
        {
            double norm = 0.0;
            for (const std::complex<double>& state : states)
            {
                norm += std::norm(state);
            }
            if (norm == 0)
            {
                throw std::runtime_error("Cannot measure zero norm state");
            }

            static std::random_device rd;
            static std::mt19937 eng(rd());
            std::uniform_real_distribution<double> distr(0.0, 1.0);

            double rnd = distr(eng);
            double cumulative_probability = 0.0;

            for (int i = 0; i < size; ++i)
            {
                cumulative_probability += std::norm(states[i]) / norm;
                if (rnd <= cumulative_probability)
                {
                    std::cout << "Measured state: " << states[i] << " at index " << i << std::endl;
                    return;
                }
            }
        }

        /**
         * @brief Normalizes the quantum states in the array.
         * 
         * This method adjusts the states so that their total probability equals 1.
         * @throws std::runtime_error if the norm of the states is zero.
         */
        constexpr void normalize()
        {
            double norm = 0.0;
            for (const auto& state : states)
            {
                norm += std::norm(state);
            }
            if (norm == 0)
            {
                throw std::runtime_error("Cannot normalize zero norm state");
            }

            for (auto& state : states)
            {
                state /= std::sqrt(norm);
            }
        }

        /**
         * @brief Prints the quantum states.
         * 
         * Outputs the index and value of each state in the quantum array.
         */
        constexpr void print() const
        {
            for (int i = 0; i < size; ++i)
            {
                std::cout << "State [" << i << "] = " << states[i] << std::endl;
            }
        }

        /**
         * @brief Copy constructor
         */
        constexpr QuantumArray(const QuantumArray& other) : states(other.states), size(other.size) {}

        /**
         * @brief Move constructor
         */
        constexpr QuantumArray(QuantumArray&& other) noexcept : states(std::move(other.states)), size(other.size) 
        {
            other.size = 0;
        }

        constexpr QuantumArray& operator=(const QuantumArray& other) noexcept
        {
            if (this != &other)
            {
                states = other.states;
                size = other.size;
            }
            return *this;
        }

        constexpr QuantumArray& operator=(QuantumArray&& other) noexcept
        {
            if (this != &other)
            {
                states = std::move(other.states);
                size = other.size;
                other.size = 0;
            }
            return *this;
        }

        /**
         * @brief Overloads the output stream operator for QuantumArray.
         * 
         * @param os The output stream.
         * @param array The QuantumArray to output.
         * @return A reference to the output stream.
         */
        constexpr friend std::ostream& operator<<(std::ostream& os, const QuantumArray& array)
        {
            for (int i = 0; i < array.size; ++i)
            {
                os << "State[" << i << "] = " << array.states[i] << '\n';
            }
            return os;
        }

    private:
        std::vector<std::complex<double>> states;
        int size;
    }; // class QuantumArray
} // namespace nstd
