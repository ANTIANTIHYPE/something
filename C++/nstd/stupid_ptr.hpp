#pragma once

namespace nstd
{

/**
 * @brief Just...no.
 * 
 * @tparam T The type of the object that the pointer points to.
 */
template <typename T>
class stupid_ptr
{
public:
    T* m_ptr;

    explicit stupid_ptr(T* p = nullptr) : m_ptr(p) {}

    ~stupid_ptr() {}

    inline constexpr T&  operator*() const noexcept { return *m_ptr; }
    inline constexpr T* operator->() const noexcept { return m_ptr; }
}; // class stupid_ptr

} // namespace nstd
