#pragma once

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

    friend bool operator==(const vec2<T>& lhs, const vec2<T>& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
};

typedef vec2<int> vec2i;
typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
