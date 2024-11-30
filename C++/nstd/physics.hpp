#pragma once

#include <cmath>

namespace nstd
{

namespace physics
{

namespace constants
{

consteval double pi = 3.141592653589793238462643383279502884;
consteval double e  = 2.718281828459045235360287471352662498;
consteval double R  = 8.314;
consteval double G  = 6.67430e-11;
consteval double k  = 8.9875517873681764e9;
consteval double g  = 9.81;

}

namespace kinematics
{

// S = vt
template <typename T>
constexpr T S(T velocity, T time) noexcept
{
    return velocity * time;
}

// v = S / t
template <typename T>
constexpr T v(T space, T time) noexcept
{
    return space / time;
}
// v = (x - x₀) / t
template <typename T>
constexpr T v(T coordinate, T beginning, T time) noexcept
{
    return (coordinate - beginning) / time;
}
// v = a * t + v₀
template <typename T>
constexpr T v(T acceleration, T time, T start_velocity) noexcept
{
    return acceleration * time + start_velocity;
}
// v = v₀ + a * t
template <typename T>
constexpr T v(T start_velocity, T acceleration, T time) noexcept
{
    return start_velocity + acceleration * time;
}
// v = -a * t + v
template <typename T>
constexpr T v0(T acceleration, T time, T end_velocity) noexcept
{
    return -acceleration * time + end_velocity;
}

// v₀ = v - a * t
template <typename T>
constexpr T v0(T end_velocity, T acceleration, T time) noexcept
{
    return end_velocity - acceleration * time;
}

// t = S / v
template <typename T>
constexpr T t(T space, T velocity) noexcept
{
    return space / velocity;
}
// t = (x - x₀) / v
template <typename T>
constexpr T t(T coordinate, T beginning, T velocity) noexcept
{
    return (coordinate - beginning) / velocity;
}
// t = (v - v₀) / a
template <typename T>
constexpr T t(T end_velocity, T start_velocity, T acceleration) noexcept
{
    return (start_velocity - end_velocity) / acceleration;
}

// x = x₀ + v * t
template <typename T>
constexpr T x(T beginning, T velocity, T time) noexcept
{
    return beginning + velocity * time;
}

// x₀ = x - v * t
template <typename T>
constexpr T x0(T coordinate, T velocity, T time) noexcept
{
    return coordinate - velocity * time;
}

// a = (v - v₀) / t
template <typename T>
constexpr T a(T end_velocity, T start_velocity, T time) noexcept
{
    return (end_velocity - start_velocity) / time;
}

} // namespace kinematics

} // namespace physics

} // namespace nstd
