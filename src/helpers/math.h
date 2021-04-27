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