#pragma once
#include <iostream>

template <typename T>
void print_col(const std::vector<T>& input)
{
    std::cout << "Vector:\n";
    for (const auto& d : input)
    {
        std::cout << "  " << static_cast<int>(d) << '\n';
    }
}

template <typename T>
void print_row(const std::vector<T>& input)
{
    std::cout << "Vector:\n";
    for (const auto& d : input)
    {
        std::cout << "  " << static_cast<int>(d) << ", ";
    }
    std::cout << '\n';
}
