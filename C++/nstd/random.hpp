#pragma once

#include <random>
#include <chrono>

namespace nstd
{

/**
 * @brief A class for generating random numbers.
 */
class random
{
public:
    /**
     * @brief Constructs a random number generator and seeds it with the current time.
     */
    random() : m_engine(std::chrono::system_clock::now().time_since_epoch().count()) {}

    /**
     * @brief Seeds the random number generator.
     * 
     * @param s The seed value.
     */
    inline void seed(std::uint32_t s) noexcept
    {
        m_engine.seed(s);
    }

    /**
     * @brief Generates a random number in the specified range.
     * 
     * @tparam T The type of the random number (must be a floating-point type).
     * 
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * 
     * @return A random number in the range [min, max].
     */
    template <typename T>
    inline T rand(T min, T max) noexcept
    {
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(m_engine);
    }

    /**
     * @brief Generates a random boolean value.
     * 
     * @return A random boolean value (true or false).
     */
    inline bool randbool() noexcept
    {
        return rand(0.0, 1.0) < 0.5;
    }

    /**
     * @brief Generates a random number from a normal distribution.
     * 
     * @param mean The mean of the normal distribution.
     * @param stddev The standard deviation of the normal distribution.
     * 
     * @return A random number from the normal distribution.
     */
    inline double randnorm(double mean, double stddev) noexcept
    {
        std::normal_distribution<double> distribution(mean, stddev);
        return distribution(m_engine);
    }

    /**
     * @brief Generates a random number using a specified distribution.
     * 
     * @tparam T The type of the random number.
     * 
     * @param distribution A reference to a uniform_real_distribution object.
     * 
     * @return A random number generated from the specified distribution.
     */
    template <typename T>
    inline auto randdist(std::uniform_real_distribution<T>& distribution) noexcept
    {
        return distribution(m_engine);
    }

    /**
     * @brief Generates a vector of random numbers in the specified range.
     * 
     * @tparam T The type of the random numbers.
     * 
     * @param size The size of the vector.
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * 
     * @return A vector containing random numbers in the range [min, max].
     */
    template <typename T>
    inline std::vector<T> randvec(int size, T min, T max) noexcept
    {
        std::vector<T> vec(size);
        for (T& value : vec)
        {
            value = rand(min, max);
        }
        return vec;
    }

    /**
     * @brief Generates a random number from an exponential distribution.
     * 
     * @param lambda The rate parameter of the exponential distribution.
     * 
     * @return A random number from the exponential distribution.
     */
    inline double randexp(double lambda) noexcept
    {
        std::exponential_distribution<double> distribution(lambda);
        return distribution(m_engine);
    }

    /**
     * @brief Generates a random number from a Poisson distribution.
     * 
     * @param mean The mean (lambda) of the Poisson distribution.
     * 
     * @return A random number from the Poisson distribution.
     */
    inline int randpoisson(double mean) noexcept
    {
        std::poisson_distribution<int> distribution(mean);
        return distribution(m_engine);
    }

private:
    std::default_random_engine m_engine;

}; // class random

} // namespace nstd
