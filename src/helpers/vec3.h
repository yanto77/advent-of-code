#pragma once

template <typename T>
struct vec3 
{ 
    T x;
    T y;
    T z;

    vec3(T X = 0, T Y = 0, T Z = 0): x(X), y(Y), z(Z) {}

    constexpr vec3<T>& operator+=(const vec3<T>& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    constexpr vec3<T>& operator-=(const vec3<T>& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    constexpr vec3<T>& operator/=(const vec3<T>& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }
    constexpr vec3<T>& operator*=(const vec3<T>& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }

    constexpr vec3<T>& operator+=(const T rhs) { x += rhs; y += rhs; z += rhs; return *this; }
    constexpr vec3<T>& operator-=(const T rhs) { x -= rhs; y -= rhs; z -= rhs; return *this; }
    constexpr vec3<T>& operator/=(const T rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }
    constexpr vec3<T>& operator*=(const T rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }

    constexpr friend vec3<T> operator+(vec3<T> lhs, const vec3<T>& rhs) { lhs += rhs; return lhs; }
    constexpr friend vec3<T> operator-(vec3<T> lhs, const vec3<T>& rhs) { lhs -= rhs; return lhs; }
    constexpr friend vec3<T> operator/(vec3<T> lhs, const vec3<T>& rhs) { lhs /= rhs; return lhs; }
    constexpr friend vec3<T> operator*(vec3<T> lhs, const vec3<T>& rhs) { lhs *= rhs; return lhs; }

    constexpr friend vec3<T> operator+(vec3<T> lhs, const T rhs) { lhs += rhs; return lhs; }
    constexpr friend vec3<T> operator-(vec3<T> lhs, const T rhs) { lhs -= rhs; return lhs; }
    constexpr friend vec3<T> operator/(vec3<T> lhs, const T rhs) { lhs /= rhs; return lhs; }
    constexpr friend vec3<T> operator*(vec3<T> lhs, const T rhs) { lhs *= rhs; return lhs; }

    constexpr friend vec3<T> operator+(const T lhs, vec3<T> rhs) { rhs += lhs; return rhs; }
    constexpr friend vec3<T> operator-(const T lhs, vec3<T> rhs) { rhs -= lhs; return rhs; }
    constexpr friend vec3<T> operator/(const T lhs, vec3<T> rhs) { rhs /= lhs; return rhs; }
    constexpr friend vec3<T> operator*(const T lhs, vec3<T> rhs) { rhs *= lhs; return rhs; }

    bool operator==(const vec3<T>& v) const { return x == v.x && y == v.y && z == v.z; }

    constexpr T& operator[](size_t idx) { return (idx == 0) ? x : ((idx == 1) ? y : z); }
    constexpr const T& operator[](size_t idx) const { return (idx == 0) ? x : ((idx == 1) ? y : z); }

    constexpr size_t sum_abs() const { return llabs(x) + llabs(y) + llabs(z); }
};

namespace std
{
    template <typename T>
    struct hash<vec3<T>>
    {   
        // Ref: https://stackoverflow.com/a/17017281
        size_t operator()( const vec3<T>& v ) const
        {
            size_t res = 17;
            res = res * 31 + hash<T>()( v.x );
            res = res * 31 + hash<T>()( v.y );
            res = res * 31 + hash<T>()( v.z );
            return res;
        }
    };
}

typedef vec3<int> vec3i;
typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
