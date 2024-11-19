#pragma once

#include <memory>
#include <atomic>

namespace nstd
{
    /**
     * @brief A smart pointer that manages the lifetime of a dynamically allocated object.
     * 
     * The `genius_ptr` class implements a shared ownership model with atomic reference counting.
     * It allows multiple pointers to share ownership of a single dynamically allocated object.
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
        constexpr genius_ptr() : ptr(nullptr), shared_count(new std::atomic<size_t>(0)), weak_count(new std::atomic<size_t>(0)) {}

        /**
         * @brief Constructs a genius_ptr that takes ownership of the given pointer.
         * @param p A pointer to a dynamically allocated object of type `T`.
         */
        constexpr explicit genius_ptr(T* p) : ptr(p), shared_count(new std::atomic<size_t>(1)), weak_count(new std::atomic<size_t>(0)) {}

        /**
         * @brief Copy constructor that creates a genius_ptr sharing ownership with another genius_ptr.
         * @param other The genius_ptr to copy from.
         */
        constexpr genius_ptr(const genius_ptr& other) : ptr(other.ptr), shared_count(other.shared_count), weak_count(other.weak_count)
        {
            if (ptr)
            {
                (*shared_count)++;
            }
        }

        /**
         * @brief Move constructor that transfers ownership from another genius_ptr.
         * @param other The genius_ptr to move from.
         */
        constexpr genius_ptr(genius_ptr&& other) noexcept : ptr(other.ptr), shared_count(other.shared_count), weak_count(other.weak_count)
        {
            other.ptr = nullptr;
            other.shared_count = nullptr;
            other.weak_count = nullptr;
        }

        ~genius_ptr() 
        {
            release();
        }

        /**
         * @brief Copy assignment operator that assigns ownership from another genius_ptr.
         * 
         * @param other The genius_ptr to assign from.
         * @return A reference to this genius_ptr.
         */
        constexpr genius_ptr& operator=(const genius_ptr& other)
        {
            if (this != &other)
            {
                release();
                ptr = other.ptr;
                shared_count = other.shared_count;
                weak_count = other.weak_count;
                if (ptr)
                {
                    (*shared_count)++;
                }
            }
            return *this;
        }

        /**
         * @brief Move assignment operator that transfers ownership from another genius_ptr.
         * 
         * @param other The genius_ptr to move from.
         * @return A reference to this genius_ptr.
         */
        constexpr genius_ptr& operator=(genius_ptr&& other) noexcept
        {
            if (this != &other)
            {
                release();
                ptr = other.ptr;
                shared_count = other.shared_count;
                weak_count = other.weak_count;
                other.ptr = nullptr;
                other.shared_count = nullptr;
                other.weak_count = nullptr;
            }
            return *this;
        }

        /**
         * @brief Retrieves the raw pointer managed by the genius_ptr.
         * @return A pointer to the managed object.
         */
        constexpr T* get() const { return ptr; }

        /**
         * @brief Dereferences the managed pointer to access the object.
         * @return A reference to the managed object.
         */
        constexpr T& operator*() const { return *ptr; }

        /**
         * @brief Accesses members of the managed object.
         * @return A pointer to the managed object.
         */
        constexpr T* operator->() const { return ptr; }

        /**
         * @brief Returns the number of shared owners of the managed object.
         * @return The number of shared_ptr instances managing the same object.
         */
        constexpr std::size_t use_count() const { return shared_count ? shared_count : 0; }

        /**
         * @brief A weak pointer that can observe the managed object without taking ownership.
         */
        class weak_ptr
        {
        public:
            /**
             * @brief Default constructor that initializes an empty weak_ptr.
             */
            constexpr weak_ptr() : ptr(nullptr), shared_count(nullptr) {}

            /**
             * @brief Constructs a weak_ptr from a genius_ptr.
             * @param gptr The genius_ptr to observe.
             */
            constexpr weak_ptr(const genius_ptr<T>& gptr) : ptr (gptr.ptr), shared_count(gptr.shared_count)
            {
                if (shared_count)
                {
                    (*shared_count)++;
                }
            }

            ~weak_ptr()
            {
                if (shared_count && --(*shared_count) == 0)
                {
                    delete shared_count;
                }
            }

            /**
             * @brief Checks if the managed object has expired (i.e., no more shared owners).
             * @return true if the managed object has expired, false otherwise.
             */
            constexpr bool expired() const { return !shared_count || *shared_count == 0; }

            /**
             * @brief Attempts to create a genius_ptr from the weak_ptr.
             * @return A genius_ptr to the managed object if it is still valid, otherwise an empty genius_ptr.
             */
            constexpr genius_ptr<T> lock() const
            {
                if (expired())
                {
                    return genius_ptr<T>();
                }
                return genius_ptr<T>(ptr);
            } // class weak_ptr

        private:
            T* ptr;
            std::atomic<std::size_t>* shared_count;
        };

        /**
         * @brief Creates a weak_ptr that observes the managed object.
         * @return A weak_ptr that observes this genius_ptr.
         */
        constexpr weak_ptr get_weak_ptr() const
        {
            return weak_ptr(*this);
        }

    private:
        T* ptr;
        std::atomic<size_t>* shared_count;
        std::atomic<size_t>* weak_count;

        /**
         * @brief Releases the managed object if this is the last owner.
         */
        constexpr void release()
        {
            if (shared_count && --(*shared_count) == 0)
            {
                delete ptr;
                delete shared_count;
            }
        }
    }; // class genius_ptr
} // namespace nstd
