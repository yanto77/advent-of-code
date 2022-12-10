#pragma once 
#include <array>

// Get a char that is visualized (with #/. pixels) in 4x6 input.
char get_letter(const std::array<char, 4*6>& input);

// AOC sometimes asks for letter inputs, but this project assumes always `size_t`
// values. Thus, sometimes the actual AOC answer is converted to custom number,
// so that we can still validate that we get proper answer.
template <size_t N>
inline size_t get_str_score(std::array<char, N> input)
{
    size_t out = 0;
    for (char ch: input)
    {
        out = 10 * out + ch;
    }
    return out;
}
