#include "setup/all.h"

ADVENT_DAY(2020, 4, 237, 172);

namespace
{
    constexpr int hex_to_dec(unsigned char hex_digit)
    {
        constexpr const signed char hex_values[256] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
            -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        };

        int value = hex_values[hex_digit];
        if (value == -1)
            throw std::invalid_argument("invalid hex digit");

        return value;
    }

    constexpr int hex_to_dec(str_view input)
    {
        return hex_to_dec(input[1]) * 1048576 // 16^5
            + hex_to_dec(input[2]) * 65536 // 16^4
            + hex_to_dec(input[3]) * 4096 // 16^3
            + hex_to_dec(input[4]) * 256 // 16^2
            + hex_to_dec(input[5]) * 16 // 16^1
            + hex_to_dec(input[6]) * 1; // 16^0
    }


    enum Field: char { byr, cid, ecl, eyr, hcl, hgt, iyr, pid };
    enum EyeColor: char { amb, blu, brn, gry, grn, hzl, oth };

    static const std::unordered_map<std::string_view, Field> s_fields =
    {
        { "byr", Field::byr }, { "cid", Field::cid }, { "ecl", Field::ecl },
        { "eyr", Field::eyr }, { "hcl", Field::hcl }, { "hgt", Field::hgt },
        { "iyr", Field::iyr }, { "pid", Field::pid }
    };

    static const std::unordered_map<std::string_view, EyeColor> s_colors =
    {
        { "amb", EyeColor::amb}, { "blu", EyeColor::blu}, { "brn", EyeColor::brn},
        { "gry", EyeColor::gry}, { "grn", EyeColor::grn}, { "hzl", EyeColor::hzl},
        { "oth", EyeColor::oth}
    };
}

result_t Day_2020_4::run_solution(str_view input) const
{
    std::bitset<8> pass_filled {};
    bool pass_valid = true;

    size_t filled_nb = 0;
    size_t valid_nb = 0;

    for_each_split(input, '\n', [&](str_view line)
    {
        if (line != "")
        {
            for (str_view field : split(line, ' '))
            {
                const auto& [key_str, val_str] = split_at(field, ':');

                const Field key = s_fields.at(key_str);
                pass_filled.set(key);

                const int value = to_int<int>(val_str);
                if (key == Field::byr) 
                {  
                    pass_valid &= (1920 <= value && value <= 2002);
                }
                else if (key == Field::iyr) 
                { 
                    pass_valid &= (2010 <= value && value <= 2020);
                }
                else if (key == Field::eyr) 
                { 
                    pass_valid &= (2020 <= value && value <= 2030);
                }
                else if (key == Field::hgt) 
                { 
                    pass_valid &=
                        (val_str.ends_with("in") && 59 <= value && value <= 76) ||
                        (val_str.ends_with("cm") && 150 <= value && value <= 193);
                }
                else if (key == Field::hcl) 
                { 
                    pass_valid &= (val_str.size() == 7 && val_str[0] == '#' && hex_to_dec(val_str) != -1);
                }
                else if (key == Field::ecl) 
                { 
                    pass_valid &= (s_colors.find(val_str) != s_colors.end());
                }
                else if (key == Field::pid) 
                { 
                    pass_valid &= (val_str.size() == 9 && value != -1);
                }
            }
        }
        else
        {
            constexpr std::bitset<8> optional {0b00000010};
            bool all_filled = (pass_filled | optional).all();

            filled_nb += static_cast<int>(all_filled);
            valid_nb += static_cast<int>(all_filled && pass_valid);

            // reset
            pass_filled.reset();
            pass_valid = true;
        }
    });

    return { filled_nb, valid_nb };
}

void Day_2020_4::run_tests() const
{
    static_assert(hex_to_dec("#000000") == 0);
    static_assert(hex_to_dec("#00000F") == 15);
    static_assert(hex_to_dec("#000010") == 16);
    static_assert(hex_to_dec("#09FBF1") == 654321);
    static_assert(hex_to_dec("#C0FFEE") == 12648430);
}
