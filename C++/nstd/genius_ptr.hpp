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
        genius_ptr() noexcept : ptr(nullptr), shared_count(0), weak_count(0) {}

        /**
         * @brief Constructs a genius_ptr that takes ownership of the given pointer.
         * 
         * @param p A pointer to a dynamically allocated object of type `T`.
         */
        explicit genius_ptr(T* p) noexcept : ptr(p), shared_count(1), weak_count(0) {}

        /**
         * @brief Copy constructor that creates a genius_ptr sharing ownership with another genius_ptr.
         * 
         * @param other The genius_ptr to copy from.
         */
        genius_ptr(const genius_ptr& other) noexcept : ptr(other.ptr), shared_count(other.shared_count.load()), weak_count(other.weak_count.load())
        {
            if (ptr)
            {
                shared_count.fetch_add(1);
            }
        }

        /**
         * @brief Move constructor that transfers ownership from another genius_ptr.
         * 
         * @param other The genius_ptr to move from.
         */
        genius_ptr(genius_ptr&& other) noexcept : ptr(other.ptr), shared_count(other.shared_count.load()), weak_count(other.weak_count.load())
        {
            other.ptr = nullptr;
            other.shared_count = 0;
            other.weak_count = 0;
        }

        /**
         * @brief Destructor that releases the managed object if this is the last owner.
         */
        ~genius_ptr() noexcept
        {
            release();
        }

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
                ptr = other.ptr;
                shared_count = other.shared_count.load();
                weak_count = other.weak_count.load();
                if (ptr)
                {
                    shared_count.fetch_add(1);
                }
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
        genius_ptr& operator=(genius_ptr&& other) noexcept
        {
            if (this != &other)
            {
                release();
                ptr = other.ptr;
                shared_count = other.shared_count.load();
                weak_count = other.weak_count.load();
                other.ptr = nullptr;
                other.shared_count = 0;
                other.weak_count = 0;
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
        genius_ptr& operator=(std::nullptr_t) noexcept
        {
            release();
            ptr = nullptr;
            shared_count = 0;
            weak_count = 0;
            return *this;
        }

        /**
         * @brief Retrieves the raw pointer managed by the genius_ptr.
         * 
         * @return A pointer to the managed object.
         */
        constexpr T* get() const noexcept { return ptr; }

        /**
         * @brief Dereferences the managed pointer to access the object.
         * 
         * @return A reference to the managed object.
         */
        constexpr T& operator*() const noexcept { return *ptr; }

        /**
         * @brief Accesses members of the managed object.
         * 
         * @return A pointer to the managed object.
         */
        constexpr T* operator->() const noexcept { return ptr; }

        /**
         * @brief Returns the number of shared owners of the managed object.
         * 
         * @return The number of genius_ptr instances managing the same object.
         */
        constexpr std::size_t use_count() const noexcept { return shared_count.load(); }

        /**
         * @brief A weak pointer that can observe the managed object without taking ownership.
         */

        class weak_ptr
        {
        public:
            /**
             * @brief Default constructor that initializes an empty weak_ptr.
             */
            weak_ptr() noexcept : ptr(nullptr), shared_count(nullptr) {}

            /**
             * @brief Constructs a weak_ptr from a genius_ptr, sharing the ownership.
             * 
             * @param gptr The genius_ptr to observe.
             */
            weak_ptr(genius_ptr<T>& gptr) noexcept : ptr(gptr.ptr), shared_count(&gptr.shared_count)
            {
                if (shared_count)
                {
                    shared_count->fetch_add(1, std::memory_order_relaxed);
                }
            }

            /**
             * @brief Destructor that decrements the weak count.
             */
            ~weak_ptr() noexcept
            {
                if (shared_count && shared_count->fetch_sub(1, std::memory_order_relaxed) == 1)
                {
                    shared_count = nullptr; // Avoid dangling pointer
                }
            }

            /**
             * @brief Checks if the managed object has expired (i.e., no shared owners).
             * 
             * @return true if the managed object has expired, false otherwise.
             */
            constexpr bool expired() const noexcept { return !shared_count || shared_count->load() == 0; }

            /**
             * @brief Attempts to create a genius_ptr from the weak_ptr.
             * 
             * @return A genius_ptr to the managed object if it is still valid, otherwise an empty genius_ptr.
             */
            constexpr genius_ptr<T> lock() const noexcept
            {
                if (expired())
                {
                    return genius_ptr<T>();
                }
                return genius_ptr<T>(ptr);
            }

        private:
            T* ptr;                                   // Pointer to the managed object.
            std::atomic<size_t>* shared_count;        // Pointer to the shared count.
        };

        /**
         * @brief Creates a weak_ptr from this genius_ptr.
         * 
         * @return A weak_ptr that observes the managed object.
         */
        constexpr weak_ptr get_weak_ptr() noexcept
        {
            return weak_ptr(*this);
        }

    private:
        T* ptr;                           // Pointer to the managed object.
        std::atomic<size_t> shared_count; // Atomic count of shared owners.
        std::atomic<size_t> weak_count;   // Atomic count of weak owners.

        /**
         * @brief Releases the managed object and decrements the reference counts.
         */
        void release() noexcept
        {
            if (shared_count.fetch_sub(1) == 1)
            {
                delete ptr;
                shared_count = 0;
                weak_count = 0; // Avoid dangling pointer
            }
        }
    }; // class genius_ptr
} // namespace nstd
