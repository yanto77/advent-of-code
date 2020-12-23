#pragma once
#include <iostream>

template<typename T>
void print(const std::vector<T>& input)
{
    std::cout << "Vector:\n";
    for (const auto& d: input)
    {
        std::cout << "  " << d << '\n';
    }
}
