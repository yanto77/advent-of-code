#pragma once
#include <helpers/math.h>

template <typename T>
struct vec2
{
    T x;
    T y;

    constexpr vec2<T>& operator+=(const vec2<T>& r) { x += r.x; y += r.y; return *this; }
    constexpr vec2<T>& operator-=(const vec2<T>& r) { x -= r.x; y -= r.y; return *this; }
    constexpr vec2<T>& operator/=(const vec2<T>& r) { x /= r.x; y /= r.y; return *this; }
    constexpr vec2<T>& operator*=(const vec2<T>& r) { x *= r.x; y *= r.y; return *this; }

    constexpr vec2<T>& operator+=(const T r) { x += r; y += r; return *this; }
    constexpr vec2<T>& operator-=(const T r) { x -= r; y -= r; return *this; }
    constexpr vec2<T>& operator/=(const T r) { x /= r; y /= r; return *this; }
    constexpr vec2<T>& operator*=(const T r) { x *= r; y *= r; return *this; }

    constexpr T norm() const { return static_cast<T>(sqrt(x*x + y*y)); };
    constexpr T norm_sq() const { return x*x + y*y; };
    
    constexpr friend vec2<T> operator+(vec2<T> l, const vec2<T>& r) { l += r; return l; }
    constexpr friend vec2<T> operator-(vec2<T> l, const vec2<T>& r) { l -= r; return l; }
    constexpr friend vec2<T> operator/(vec2<T> l, const vec2<T>& r) { l /= r; return l; }
    constexpr friend vec2<T> operator*(vec2<T> l, const vec2<T>& r) { l *= r; return l; }

    constexpr friend vec2<T> operator+(vec2<T> l, const T r) { l += r; return l; }
    constexpr friend vec2<T> operator-(vec2<T> l, const T r) { l -= r; return l; }
    constexpr friend vec2<T> operator/(vec2<T> l, const T r) { l /= r; return l; }
    constexpr friend vec2<T> operator*(vec2<T> l, const T r) { l *= r; return l; }

    constexpr friend vec2<T> operator+(const T l, vec2<T> r) { r += l; return r; }
    constexpr friend vec2<T> operator-(const T l, vec2<T> r) { r -= l; return r; }
    constexpr friend vec2<T> operator/(const T l, vec2<T> r) { r /= l; return r; }
    constexpr friend vec2<T> operator*(const T l, vec2<T> r) { r *= l; return r; }

    constexpr friend bool operator<(const vec2<T>& l, const vec2<T>& r) { return l.x < r.x && l.y < r.y; }
    constexpr friend bool operator> (const vec2<T>& l, const vec2<T>& r) { return r < l; }
    constexpr friend bool operator<=(const vec2<T>& l, const vec2<T>& r) { return l.x <= r.x && l.y <= r.y; }
    constexpr friend bool operator>=(const vec2<T>& l, const vec2<T>& r) { return !(r < l); }

    constexpr friend bool operator==(const vec2<T>& l, const vec2<T>& r) { return l.x == r.x && l.y == r.y; }
    constexpr friend bool operator!=(const vec2<T>& l, const vec2<T>& r) { return !(l == r); }

    constexpr friend int64_t dist_mnht(const vec2<T>& l, const vec2<T>& r) { return abs(l.x - r.x) + abs(l.y - r.y); }

    template <typename U>
    constexpr friend vec2<T> clamp(vec2<T> v, U lower, U upper) 
    { 
        return { ::clamp(v.x, lower, upper), ::clamp(v.y, lower, upper) }; 
    };
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
