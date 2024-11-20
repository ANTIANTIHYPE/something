#pragma once

namespace nstd
{
    /**
     * @brief A stupid_ptr implementation that wraps a raw pointer.
     * @tparam T The type of the object that the pointer points to.
     */
    template <typename T>
    class stupid_ptr
    {
    public:
        T* ptr;

        /**
         * @brief Constructs a stupid_ptr with an optional raw pointer.
         * @param p A raw pointer to an object of type T. Defaults to nullptr.
         */
        explicit stupid_ptr(T* p = nullptr) : ptr(p) {}

        /**
         * @brief Destructor for the stupid_ptr.
         * This destructor does not manage the memory of the raw pointer.
         */
        ~stupid_ptr() {}

        /**
         * @brief Dereference operator.
         * @return Reference to the object of type T.
         */
        constexpr T& operator*() noexcept
        {
            return *ptr;
        }

        /**
         * @brief Member access operator.
         * @return Pointer to the object of type T.
         */
        constexpr T* operator->() noexcept
        {
            return ptr;
        }
    }; // class stupid_ptr
} // namespace nstd
