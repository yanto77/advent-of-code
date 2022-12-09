#pragma once
#include <helpers/math.h>

template <typename T>
struct vec2
{
    T x;
    T y;

    constexpr vec2<T>& operator+=(const vec2<T>& rhs) { x += rhs.x; y += rhs.y; return *this; }
    constexpr vec2<T>& operator-=(const vec2<T>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    constexpr vec2<T>& operator/=(const vec2<T>& rhs) { x /= rhs.x; y /= rhs.y; return *this; }
    constexpr vec2<T>& operator*=(const vec2<T>& rhs) { x *= rhs.x; y *= rhs.y; return *this; }

    constexpr vec2<T>& operator+=(const T rhs) { x += rhs; y += rhs; return *this; }
    constexpr vec2<T>& operator-=(const T rhs) { x -= rhs; y -= rhs; return *this; }
    constexpr vec2<T>& operator/=(const T rhs) { x /= rhs; y /= rhs; return *this; }
    constexpr vec2<T>& operator*=(const T rhs) { x *= rhs; y *= rhs; return *this; }

    constexpr T norm() const { return static_cast<T>(sqrt(x*x + y*y)); };
    constexpr T norm_sq() const { return x*x + y*y; };
    
    constexpr friend vec2<T> operator+(vec2<T> lhs, const vec2<T>& rhs) { lhs += rhs; return lhs; }
    constexpr friend vec2<T> operator-(vec2<T> lhs, const vec2<T>& rhs) { lhs -= rhs; return lhs; }
    constexpr friend vec2<T> operator/(vec2<T> lhs, const vec2<T>& rhs) { lhs /= rhs; return lhs; }
    constexpr friend vec2<T> operator*(vec2<T> lhs, const vec2<T>& rhs) { lhs *= rhs; return lhs; }

    constexpr friend vec2<T> operator+(vec2<T> lhs, const T rhs) { lhs += rhs; return lhs; }
    constexpr friend vec2<T> operator-(vec2<T> lhs, const T rhs) { lhs -= rhs; return lhs; }
    constexpr friend vec2<T> operator/(vec2<T> lhs, const T rhs) { lhs /= rhs; return lhs; }
    constexpr friend vec2<T> operator*(vec2<T> lhs, const T rhs) { lhs *= rhs; return lhs; }

    constexpr friend vec2<T> operator+(const T lhs, vec2<T> rhs) { rhs += lhs; return rhs; }
    constexpr friend vec2<T> operator-(const T lhs, vec2<T> rhs) { rhs -= lhs; return rhs; }
    constexpr friend vec2<T> operator/(const T lhs, vec2<T> rhs) { rhs /= lhs; return rhs; }
    constexpr friend vec2<T> operator*(const T lhs, vec2<T> rhs) { rhs *= lhs; return rhs; }

    constexpr friend bool operator==(const vec2<T>& lhs, const vec2<T>& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }

    template <typename U>
    constexpr friend vec2<T> clamp(vec2<T> v, U lower, U upper) { return { ::clamp(v.x, lower, upper), ::clamp(v.y, lower, upper) }; };
};

namespace std
{
    template <typename T>
    struct hash<vec2<T>>
    {
        // Ref: https://stackoverflow.com/a/17017281
        size_t operator()( const vec2<T>& v ) const
        {
            size_t res = 17;
            res = res * 31 + hash<T>()( v.x );
            res = res * 31 + hash<T>()( v.y );
            return res;
        }
    };
}

typedef vec2<int32_t> vec2i;
typedef vec2<uint32_t> vec2u;
typedef vec2<float> vec2f;
typedef vec2<double> vec2d;

typedef vec2<int8_t> vec2i8;
typedef vec2<uint8_t> vec2u8;
