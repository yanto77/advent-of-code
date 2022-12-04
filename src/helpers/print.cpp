#include "print.h"

void print_bits(uint8_t* data, size_t count, bool color)
{
    for (size_t i = 0; i < count; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            bool is_set = get_bit(data[i], j);
            if (color)
            {
                const char* one = COLOR_GREEN() "1" COLOR_RESET();
                const char* zero = COLOR_GRAY() "0" COLOR_RESET();
                printf("%s", is_set ? one : zero);
            }
            else
            {
                printf(is_set ? "1" : "0");
            }
        }
        fmt::print(" ");
    }
    fmt::print("\n");
}

void print_bits_ext(uint8_t* data, size_t count, bool color)
{
    fmt::print("\n");

    fmt::print("  ");
    print_bits(data, count, color);

    fmt::print("  ");
    for (size_t i = 0; i < count; i++)
    {
        fmt::print("0x{:<7x}", data[i]);
    }
    fmt::print("\n");

    fmt::print("  ");
    for (size_t i = 0; i < count; i++)
    {
        bool printable = (0x20 <= data[i] && data[i] <= 0x7e);
        char ch = printable ? (char)data[i] : '?';
        fmt::print("{:<9}", ch);
    }
    fmt::print("\n");
}
