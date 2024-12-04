#pragma once

#include <iostream>
#include <complex>
#include <random>

namespace nstd
{

/**
 * A class for the representation and manipulation of quantum m_states
 * using complex numbers.
 */
class QuantumArray
{
public:
    /**
     * @brief Constructs a QuantumArray with a given m_size.
     * 
     * @param n The m_size of the quantum array.
     */
    QuantumArray(int n) : m_size(n)
    {
        if (n <= 0)
        {
            throw std::invalid_argument("Size must be positive");
        }
        m_states.resize(n, {0, 0});
    }

    /**
     * @brief Initializes the quantum array to a specific state.
     * 
     * @param state The complex state to set all elements to.
     */
    inline void initialize(const std::complex<double>& state)
    {
        std::fill(m_states.begin(), m_states.end(), state);
    }

    /**
     * @brief Sets the state at a specific index.
     * 
     * @param index The index of the state to set.
     * @param state The complex state to set.
     * 
     * @throws std::out_of_range if the index is out of bounds.
     */
    inline void set(int index, const std::complex<double>& state)
    {
        if (index < 0 || index >= m_size)
        {
            throw std::out_of_range("Index out of range");
        }
        m_states[index] = state;
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
    inline std::complex<double> get(int index) const
    {
        if (index < 0 || index >= m_size)
        {
            throw std::out_of_range("Index out of range");
        }
        return m_states[index];
    }

    /**
     * @brief Measures the quantum state and outputs the result.
     * 
     * The measurement is probabilistic based on the square of the amplitude of each state.
     * 
     * @throws std::runtime_error if the norm of the m_states is zero.
     */
    inline void measure()
    {
        double norm = 0.0;
        for (const std::complex<double>& state : m_states)
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

        for (int i = 0; i < m_size; ++i)
        {
            cumulative_probability += std::norm(m_states[i]) / norm;
            if (rnd <= cumulative_probability)
            {
                std::cout << "Measured state: " << m_states[i] << " at index " << i << std::endl;
                return;
            }
        }
    }

    /**
     * @brief Normalizes the quantum m_states in the array.
     * 
     * This method adjusts the m_states so that their total probability equals 1.
     * 
     * @throws std::runtime_error if the norm of the m_states is zero.
     */
    inline void normalize()
    {
        double norm = 0.0;
        for (const std::complex<double>& state : m_states)
        {
            norm += std::norm(state);
        }

        if (norm == 0)
        {
            throw std::runtime_error("Cannot normalize zero norm state");
        }

        for (std::complex<double>& state : m_states)
        {
            state /= std::sqrt(norm);
        }
    }

    /**
     * @brief Applies the Hadamard gate to the quantum m_states.
     * 
     * @tparam T The numeric type of the quantum state amplitudes, must support basic arithmetic operations.
     * 
     * @param m_states A reference to a vector of complex numbers representing the quantum m_states.
     * 
     * @throws std::invalid_argument if the m_size of the m_states vector is not a power of 2.
     */
    template <typename T>
    inline void hadamard(std::vector<std::complex<T>>& m_states)
    {
        const std::size_t m_size = m_states.m_size();

        if (m_size == 0 || (m_size & (m_size - 1)) != 0)
        {
            throw std::invalid_argument("The m_size of the m_states vector must be a power of 2.");
        }

        std::vector<std::complex<T>> new_states(m_size);

        for (std::size_t i = 0; i < m_size; ++i)
        {
            std::size_t partner = i ^ (m_size / 2);
            new_states[i] = (m_states[i] + m_states[partner]) / std::sqrt(static_cast<T>(2));
        }

        m_states = std::move(new_states);
    }

    /**
     * @brief Applies the Pauli-X gate to the quantum m_states.
     * 
     * The Pauli-X gate flips the state at the specified index. If the state is |0⟩, it becomes |1⟩, and vice versa.
     * 
     * @tparam T The numeric type of the quantum state amplitudes, must support basic arithmetic operations.
     * 
     * @param m_states A reference to a vector of complex numbers representing the quantum m_states.
     * @param index The index of the state to flip.
     * 
     * @throws std::out_of_range if the index is out of bounds.
     */
    template <typename T>
    inline void PauliX(std::vector<std::complex<T>>& m_states, int index)
    {
        if (index < 0 || index >= static_cast<int>(m_states.m_size()))
        {
            throw std::out_of_range("Index out of range");
        }

        m_states[index] = (m_states[index] == std::complex<T>(0, 0)) ? std::complex<T>(1, 0) : std::complex<T>(0, 0);
    }

    /**
     * @brief Prints the quantum m_states.
     * 
     * Outputs the index and value of each state in the quantum array.
     */
    inline void print() const noexcept
    {
        for (int i = 0; i < m_size; ++i)
        {
            std::cout << "State[" << i << "] = " << m_states[i] << std::endl;
        }
    }

    /**
     * @brief Copy constructor
     */
    QuantumArray(const QuantumArray& other) : m_states(other.m_states), m_size(other.m_size) {}

    /**
     * @brief Move constructor
     */
    QuantumArray(QuantumArray&& other) noexcept : m_states(std::move(other.m_states)), m_size(other.m_size) { other.m_size = 0; }

    inline QuantumArray& operator=(const QuantumArray& other) noexcept
    {
        if (this != &other)
        {
            m_states = other.m_states;
            m_size = other.m_size;
        }
        return *this;
    }

    inline QuantumArray& operator=(QuantumArray&& other) noexcept
    {
        if (this != &other)
        {
            m_states = std::move(other.m_states);
            m_size = other.m_size;
            other.m_size = 0;
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
    inline friend std::ostream& operator<<(std::ostream& os, const QuantumArray& array)
    {
        for (int i = 0; i < array.m_size; ++i)
        {
            os << "State[" << i << "] = " << array.m_states[i] << '\n';
        }
        return os;
    }

private:
    std::vector<std::complex<double>> m_states; // Vector to hold the complex m_states of the quantum array
    int m_size;                                 // The number of m_states in the quantum array
}; // class QuantumArray

} // namespace nstd
