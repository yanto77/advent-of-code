#pragma once

template <typename T>
inline T min(const T& a, const T& b)
{
    return (a < b) ? a : b;
}

template <typename T>
inline T max(const T& a, const T& b)
{
    return (a > b) ? a : b;
}

template <typename T>
inline T range(const T& val, const T& min, const T& max)
{
    if (val <= min)
        return min;
    else if (val >= max)
        return max;
    else
        return val;
}

template <typename T>
inline bool in_range(const T& val, const T& min, const T& max)
{
    return (min <= val && val <= max);
}

// Greatest Common Divisor
inline int64_t gcd(int64_t a, int64_t b)
{
    int64_t t;
    while (b)
    {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Least (Lowest) Common Multiple
inline int64_t lcm(int64_t a, int64_t b)
{
    return a / gcd(a, b) * b;
}
