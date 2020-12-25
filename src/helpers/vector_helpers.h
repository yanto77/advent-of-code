#pragma once

struct vec2i
{
    int x;
    int y;

    constexpr vec2i& operator+=(const vec2i& rhs) { x += rhs.x; y += rhs.y; return *this; }
    constexpr vec2i& operator-=(const vec2i& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    constexpr vec2i& operator/=(const vec2i& rhs) { x /= rhs.x; y /= rhs.y; return *this; }
    constexpr vec2i& operator*=(const vec2i& rhs) { x *= rhs.x; y *= rhs.y; return *this; }

    constexpr vec2i& operator+=(const int rhs) { x += rhs; y += rhs; return *this; }
    constexpr vec2i& operator-=(const int rhs) { x -= rhs; y -= rhs; return *this; }
    constexpr vec2i& operator/=(const int rhs) { x /= rhs; y /= rhs; return *this; }
    constexpr vec2i& operator*=(const int rhs) { x *= rhs; y *= rhs; return *this; }

    constexpr friend vec2i operator+(vec2i lhs, const vec2i& rhs) { lhs += rhs; return lhs; }
    constexpr friend vec2i operator-(vec2i lhs, const vec2i& rhs) { lhs -= rhs; return lhs; }
    constexpr friend vec2i operator/(vec2i lhs, const vec2i& rhs) { lhs /= rhs; return lhs; }
    constexpr friend vec2i operator*(vec2i lhs, const vec2i& rhs) { lhs *= rhs; return lhs; }

    constexpr friend vec2i operator+(vec2i lhs, const int rhs) { lhs += rhs; return lhs; }
    constexpr friend vec2i operator-(vec2i lhs, const int rhs) { lhs -= rhs; return lhs; }
    constexpr friend vec2i operator/(vec2i lhs, const int rhs) { lhs /= rhs; return lhs; }
    constexpr friend vec2i operator*(vec2i lhs, const int rhs) { lhs *= rhs; return lhs; }
};
