#pragma once
#include <cstddef>

struct result_t
{
    size_t part1 = 0;
    size_t part2 = 0;
};

inline bool operator==(const result_t& lhs, const result_t& rhs) 
{ 
    if (lhs.part1 == 0 || lhs.part2 == 0 || rhs.part1 == 0 || rhs.part2 == 0)
        return false; // solution is not ready

    return (lhs.part1 == rhs.part1 && lhs.part2 == rhs.part2);
}

inline bool operator!=(const result_t& lhs, const result_t& rhs) 
{ 
    return !(lhs == rhs); 
}