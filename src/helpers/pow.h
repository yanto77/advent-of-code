#pragma once
#include <cstdint>

/// Power function helpers

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
