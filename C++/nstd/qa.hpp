#pragma once

#include <iostream>
#include <complex>
#include <random>

namespace nstd
{
    /**
     * A class for the representation and manipulation of quantum states
     * using complex numbers.
     */
    class QuantumArray
    {
    public:
        /**
         * @brief Constructs a QuantumArray with a given size.
         * 
         * @param n The size of the quantum array.
         */
        QuantumArray(int n) : size(n)
        {
            if (n <= 0)
            {
                throw std::invalid_argument("Size must be positive");
            }
            states.resize(n, {0, 0});
        }

        /**
         * @brief Initializes the quantum array to a specific state.
         * 
         * @param state The complex state to set all elements to.
         */
        void initialize(const std::complex<double>& state)
        {
            std::fill(states.begin(), states.end(), state);
        }

        /**
         * @brief Sets the state at a specific index.
         * 
         * @param index The index of the state to set.
         * @param state The complex state to set.
         * 
         * @throws std::out_of_range if the index is out of bounds.
         */
        void set(int index, const std::complex<double>& state)
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
         * 
         * @return The complex state at the specified index.
         * 
         * @throws std::out_of_range if the index is out of bounds.
         */
        std::complex<double> get(int index) const
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
         * 
         * @throws std::runtime_error if the norm of the states is zero.
         */
        void measure()
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
         * 
         * @throws std::runtime_error if the norm of the states is zero.
         */
        void normalize()
        {
            double norm = 0.0;
            for (const std::complex<double>& state : states)
            {
                norm += std::norm(state);
            }
            if (norm == 0)
            {
                throw std::runtime_error("Cannot normalize zero norm state");
            }

            for (std::complex<double>& state : states)
            {
                state /= std::sqrt(norm);
            }
        }

        /**
         * @brief Applies the Hadamard gate to the quantum states.
         * 
         * The Hadamard gate transforms the states into superpositions.
         */
        void Hadamard()
        {
            for (int i = 0; i < size; ++i)
            {
                std::complex<double> state = states[i];
                states[i] = (state + states[i ^ 1]) / std::sqrt(2.0); // Assuming size is a power of 2
            }
        }

        /**
         * @brief Applies the Pauli-X gate to the quantum states.
         * 
         * The Pauli-X gate flips the state at the specified index.
         * 
         * @param index The index of the state to flip.
         * 
         * @throws std::out_of_range if the index is out of bounds.
         */
        void PauliX(int index)
        {
            if (index < 0 || index >= size)
            {
                throw std::out_of_range("Index out of range");
            }
            states[index] = states[index] == std::complex<double>(0, 0) ? std::complex<double>(1, 0) : std::complex<double>(0, 0);
        }

        /**
         * @brief Prints the quantum states.
         * 
         * Outputs the index and value of each state in the quantum array.
         */
        void print() const
        {
            for (int i = 0; i < size; ++i)
            {
                std::cout << "State[" << i << "] = " << states[i] << std::endl;
            }
        }

        /**
         * @brief Copy constructor
         */
        QuantumArray(const QuantumArray& other) : states(other.states), size(other.size) {}

        /**
         * @brief Move constructor
         */
        QuantumArray(QuantumArray&& other) noexcept : states(std::move(other.states)), size(other.size) 
        {
            other.size = 0;
        }

        QuantumArray& operator=(const QuantumArray& other) noexcept
        {
            if (this != &other)
            {
                states = other.states;
                size = other.size;
            }
            return *this;
        }

        QuantumArray& operator=(QuantumArray&& other) noexcept
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
         * 
         * @return A reference to the output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const QuantumArray& array)
        {
            for (int i = 0; i < array.size; ++i)
            {
                os << "State[" << i << "] = " << array.states[i] << '\n';
            }
            return os;
        }

    private:
        std::vector<std::complex<double>> states; // Vector to hold the complex states of the quantum array
        int size;                                 // The number of states in the quantum array
    }; // class QuantumArray
} // namespace nstd
