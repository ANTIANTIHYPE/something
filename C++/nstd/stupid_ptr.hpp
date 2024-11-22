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

        explicit stupid_ptr(T* p = nullptr) : ptr(p) {}

        ~stupid_ptr() {}

        constexpr T& operator*() noexcept
        {
            return *ptr;
        }

        constexpr T* operator->() noexcept
        {
            return ptr;
        }
    }; // class stupid_ptr
} // namespace nstd
