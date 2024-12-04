#ifndef COROUTINE_HPP
#define COROUTINE_HPP

#include <iostream>
#include <functional>
#include <exception>

#if __cplusplus > 201703L

#include <coroutine>

#endif // ^^^ C++20 ^^^

namespace nstd
{

/**
* @brief Abstract base class for coroutines.
*/
class CoroutineBase
{
public:
    virtual ~CoroutineBase() = default;

    /**
     * @brief Resumes the coroutine execution.
     * 
     * @return true if the coroutine was resumed successfully, false if it has finished.
     */
    virtual bool resume() = 0;
};

#if __cplusplus <= 201703L

/**
 * @brief Coroutine class
 */
class coroutine : public CoroutineBase
{
public:
    /**
     * @brief Constructs a coroutine with a given function.
     * 
     * @param func The function to be executed by the coroutine.
     */
    coroutine(std::function<void()> func)
        : func_(func), finished_(false) {}

    /**
     * @brief Resumes the coroutine execution.
     * 
     * @return true if the coroutine was resumed successfully, false if it has finished.
     * 
     * @throws CoroutineException if an error occurs during execution.
     */
    inline bool resume() override
    {
        if (finished_) return false;
        try { func_(); }
        catch (...)
        {
            std::cerr << "Error in coroutine" << std::endl;
            finished_ = true;
            throw CoroutineException();
        }
        finished_ = true;
        return true;
    }

private:
    std::function<void()> func_; // The function to be executed by the coroutine.
    bool finished_;              // Indicates whether the coroutine has finished execution.
};

#else // ^^^ C++98 to C++17 ^^^ VVV C++20 VVV

/**
 * @brief Coroutine structure
 */
struct coroutine
{
    /**
     * @brief Constructs a coroutine with a given function.
     */
    struct promise_type
    {
        int m_curval;                  // The current value yielded by the coroutine.
        int return_value_;             // The value returned from the coroutine upon completion.
        std::exception_ptr exception_; // Holds a pointer to any unhandled exception that may occur during the execution of the coroutine.

        /**
         * @brief Provides the initial suspend behavior of the coroutine.
         * 
         * @return A suspend operation.
         */
        inline std::suspend_always initial_suspend() { return std::suspend_always(); }

        /**
         * @brief Provides the final suspend behavior of the coroutine.
         * 
         * @return A suspend operation.
         */
        inline std::suspend_always final_suspend() noexcept { return std::suspend_always(); }

        /**
         * @brief Retrieves the coroutine object.
         * 
         * @return The coroutine object.
         */
        inline coroutine get_return_object() noexcept
        {
            return coroutine{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        /**
         * @brief Yields a value from the coroutine.
         * 
         * @param value The value to yield.
         * 
         * @return A suspend operation.
         */
        inline std::suspend_always yield_value(int value)
        {
            m_curval = value;
            return {};
        }

        /**
         * @brief Returns a value from the coroutine.
         * 
         * @param value The value to return.
         * 
         * @return A suspend operation.
         */
        inline std::suspend_always return_value(int value)
        {
            return_value_ = value;
            return {};
        }

        /**
         * @brief Handles any unhandled exceptions.
         */
        inline void unhandled_exception()
        {
            exception_ = std::current_exception();
        }
    };

    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro; // The coroutine handle that manages the execution of the coroutine.

    /**
     * @brief Constructs a coroutine with a given handle.
     * 
     * @param h The coroutine handle.
     */
    coroutine(handle_type h) : coro(h) {}
        
    /**
     * @brief Destroys the coroutine.
     */
    ~coroutine()
    {
        if (coro) coro.destroy();
    }

    /**
     * @brief Resumes the coroutine execution.
     * 
     * @return true if the coroutine was resumed successfully, false if it has finished.
     * 
     * @throws std::exception if an unhandled exception occurs during execution.
     */
    inline bool resume() const
    {
        if (coro)
        {
            coro.resume();
            if (coro.promise().exception_)
            {
                std::rethrow_exception(coro.promise().exception_);
            }
            return !coro.done();
        }
        return false;
    }

    /**
     * @brief Retrieves the current value yielded by the coroutine.
     * 
     * @return The current value.
     */
    inline int current_value()
    {
        return coro.promise().m_curval;
    }

    /**
     * @brief Retrieves the return value from the coroutine.
     * 
     * @return The return value.
     */
    const inline int return_value() const
    {
        return coro.promise().return_value_;
    }
};

#endif // ^^^ C++20 ^^^

/**
 * @brief Exception class for coroutine errors.
 */
class CoroutineException : public std::exception
{
public:
    /**
     * @brief Returns a description of the exception.
     * 
     * @return A C-style string describing the error.
     */
    const char* what() const noexcept override
    {
        return "Coroutine exception occurred";
    }
};

} // namespace nstd

#endif // COROUTINE_HPP
