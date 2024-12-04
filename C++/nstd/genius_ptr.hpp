#pragma once

#include <memory>
#include <atomic>

namespace nstd
{

/**
 * @brief weak_ptr, smart_ptr and shared_ptr combined
 * 
 * @tparam T The type of the object managed by the smart pointer.
 */
template <typename T>
class genius_ptr
{
public:
    /**
     * @brief Default constructor that initializes an empty genius_ptr.
     */
    genius_ptr() noexcept : m_ptr(nullptr), m_sharedCount(0), m_weakCount(0) {}

    /**
     * @brief Constructs a genius_ptr that takes ownership of the given pointer.
     * 
     * @param p A pointer to a dynamically allocated object of type `T`.
     */
    explicit genius_ptr(T* p) noexcept : m_ptr(p), m_sharedCount(1), m_weakCount(0) {}

    /**
     * @brief Copy constructor that creates a genius_ptr sharing ownership with another genius_ptr.
     * 
     * @param other The genius_ptr to copy from.
     */
    genius_ptr(const genius_ptr& other) noexcept : m_ptr(other.m_ptr), m_sharedCount(other.m_sharedCount.load()), m_weakCount(other.m_weakCount.load())
    {
        if (m_ptr) m_sharedCount.fetch_add(1);
    }

    /**
     * @brief Move constructor that transfers ownership from another genius_ptr.
     * 
     * @param other The genius_ptr to move from.
     */
    genius_ptr(genius_ptr&& other) noexcept : m_ptr(other.m_ptr), m_sharedCount(other.m_sharedCount.load()), m_weakCount(other.m_weakCount.load())
    {
        other.m_ptr = nullptr;
        other.m_sharedCount = 0;
        other.m_weakCount = 0;
    }

    /**
     * @brief Destructor that releases the managed object if this is the last owner.
     */
    ~genius_ptr() noexcept { release(); }

    /**
     * @brief Copy assignment operator that assigns ownership from another genius_ptr.
     * 
     * @param other The genius_ptr to assign from.
     * 
     * @return A reference to this genius_ptr.
     */
    genius_ptr& operator=(const genius_ptr& other) noexcept
    {
        if (this != &other)
        {
            release();
            m_ptr = other.m_ptr;
            m_sharedCount = other.m_sharedCount.load();
            m_weakCount = other.m_weakCount.load();
            if (m_ptr) m_sharedCount.fetch_add(1);
        }
        return *this;
    }

    /**
     * @brief Move assignment operator that transfers ownership from another genius_ptr.
     * 
     * @param other The genius_ptr to move from.
     * 
     * @return A reference to this genius_ptr.
     */
    inline genius_ptr& operator=(genius_ptr&& other) noexcept
    {
        if (this != &other)
        {
            release();
            m_ptr = other.m_ptr;
            m_sharedCount = other.m_sharedCount.load();
            m_weakCount = other.m_weakCount.load();
            other.m_ptr = nullptr;
            other.m_sharedCount = 0;
            other.m_weakCount = 0;
        }
        return *this;
    }

    /**
     * @brief Assignment operator that allows assignment from nullptr.
     * 
     * @param nullptr_t A nullptr to assign to this genius_ptr.
     * 
     * @return A reference to this genius_ptr.
     */
    inline genius_ptr& operator=(std::nullptr_t) noexcept
    {
        release();
        m_ptr = nullptr;
        m_sharedCount = 0;
        m_weakCount = 0;
        return *this;
    }

    /**
     * @brief Retrieves the raw pointer managed by the genius_ptr.
     * 
     * @return A pointer to the managed object.
     */
    constexpr inline T* get() const noexcept { return m_ptr; }

    /**
     * @brief Dereferences the managed pointer to access the object.
     * 
     * @return A reference to the managed object.
     */
    constexpr inline T& operator*() const noexcept { return *m_ptr; }

    /**
     * @brief Accesses members of the managed object.
     * 
     * @return A pointer to the managed object.
     */
    constexpr inline T* operator->() const noexcept { return m_ptr; }

    /**
     * @brief Returns the number of shared owners of the managed object.
     * 
     * @return The number of genius_ptr instances managing the same object.
     */
    constexpr inline std::size_t use_count() const noexcept { return m_sharedCount.load(); }

    /**
     * @brief A weak pointer that can observe the managed object without taking ownership.
     */
    class weak_ptr
    {
    public:
        /**
         * @brief Default constructor that initializes an empty weak_ptr.
         */
        weak_ptr() noexcept : m_ptr(nullptr), m_sharedCount(nullptr) {}

        /**
         * @brief Constructs a weak_ptr from a genius_ptr, sharing the ownership.
         * 
         * @param gptr The genius_ptr to observe.
         */
        weak_ptr(genius_ptr<T>& gptr) noexcept : m_ptr(gptr.m_ptr), m_sharedCount(&gptr.m_sharedCount)
        {
            if (m_sharedCount)
            {
                m_sharedCount->fetch_add(1, std::memory_order_relaxed);
            }
        }

        /**
         * @brief Destructor that decrements the weak count.
         */
        ~weak_ptr() noexcept
        {
            if (m_sharedCount && m_sharedCount->fetch_sub(1, std::memory_order_relaxed) == 1)
            {
                m_sharedCount = nullptr; // Avoid dangling pointer
            }
        }

        /**
         * @brief Checks if the managed object has expired (i.e., no shared owners).
         * 
         * @return true if the managed object has expired, false otherwise.
         */
        constexpr inline bool expired() const noexcept { return !m_sharedCount || m_sharedCount->load() == 0; }

        /**
         * @brief Attempts to create a genius_ptr from the weak_ptr.
         * 
         * @return A genius_ptr to the managed object if it is still valid, otherwise an empty genius_ptr.
         */
        constexpr inline genius_ptr<T> lock() const noexcept
        {
            if (expired()) return genius_ptr<T>();

            return genius_ptr<T>(m_ptr);
        }

    private:
        T* m_ptr;                                // Pointer to the managed object.
        std::atomic<std::size_t>* m_sharedCount; // Pointer to the shared count.
    };

    /**
     * @brief Creates a weak_ptr from this genius_ptr.
     * 
     * @return A weak_ptr that observes the managed object.
     */
    constexpr inline weak_ptr get_weak_ptr() noexcept { return weak_ptr(*this); }

private:
    T* m_ptr;                               // Pointer to the managed object.
    std::atomic<std::size_t> m_sharedCount; // Atomic count of shared owners.
    std::atomic<std::size_t> m_weakCount;   // Atomic count of weak owners.

    /**
     * @brief Releases the managed object and decrements the reference counts.
     */
    void inline release() noexcept
    {
        if (m_sharedCount.fetch_sub(1) == 1)
        {
            delete m_ptr;
            m_sharedCount = 0;
            m_weakCount = 0; // Avoid dangling pointer
        }
    }
}; // class genius_ptr

} // namespace nstd
