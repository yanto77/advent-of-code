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


// Computes a^b (mod m)
// Ref: https://cp-algorithms.com/algebra/binary-exp.html
inline int64_t binpow(int64_t a, int64_t b, int64_t m)
{
    a %= m;
    int64_t res = 1;
    while (b > 0)
    {
        if (b & 1)
            res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

// Computes a^b
// Ref: https://cp-algorithms.com/algebra/binary-exp.html
inline int64_t binpow(int64_t a, int64_t b)
{
    int64_t res = 1;
    while (b > 0)
    {
        if (b & 1)
            res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}
