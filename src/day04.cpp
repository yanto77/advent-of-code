#include "advent2020.h"
#include <iostream>

namespace
{
    // constexpr bool is_bit_set(uint8_t num, int bit)
    // {
    //     return ((num >> bit) & 1) == 1;
    // }

    constexpr void set_bit(uint8_t& trg, int bit)
    {
        trg |= 1 << (bit);
    }

    // void print_bits(uint8_t trg)
    // {
    //     for (int i = 0; i < 8; ++i)
    //     {
    //         printf(is_bit_set(trg, i) ? "1" : "0");
    //     }
    // }

    class passport_t
    {
        private:
            enum Field: char
            {
                byr,
                cid,
                ecl,
                eyr,
                hcl,
                hgt,
                iyr,
                pid
            };

        public:
            bool is_valid() const
            {
                constexpr uint8_t optional = 0b00000010;
                constexpr uint8_t expected = 0b11111111;
                return (m_filled | optional) == expected;
            }

            // bool is_fully_valid() const
            // {
            //     return is_valid();
            // }

            void set_byr(const std::string_view& sv)
            {
                m_byr = sv;
                set_bit(m_filled, Field::byr);
            }

            void set_cid(const std::string_view& sv)
            {
                m_cid = sv;
                set_bit(m_filled, Field::cid);
            }

            void set_ecl(const std::string_view& sv)
            {
                m_ecl = sv;
                set_bit(m_filled, Field::ecl);
            }

            void set_eyr(const std::string_view& sv)
            {
                m_eyr = sv;
                set_bit(m_filled, Field::eyr);
            }

            void set_hcl(const std::string_view& sv)
            {
                m_hcl = sv;
                set_bit(m_filled, Field::hcl);
            }

            void set_hgt(const std::string_view& sv)
            {
                m_hgt = sv;
                set_bit(m_filled, Field::hgt);
            }

            void set_iyr(const std::string_view& sv)
            {
                m_iyr = sv;
                set_bit(m_filled, Field::iyr);
            }

            void set_pid(const std::string_view& sv)
            {
                m_pid = sv;
                set_bit(m_filled, Field::pid);
            }

            // void print() const
            // {
            //     std::cout << "pass: " << std::endl;
            //     std::cout << "    byr: " << m_byr << std::endl;
            //     std::cout << "    cid: " << m_cid << std::endl;
            //     std::cout << "    ecl: " << m_ecl << std::endl;
            //     std::cout << "    eyr: " << m_eyr << std::endl;
            //     std::cout << "    hcl: " << m_hcl << std::endl;
            //     std::cout << "    hgt: " << m_hgt << std::endl;
            //     std::cout << "    iyr: " << m_iyr << std::endl;
            //     std::cout << "    pid: " << m_pid << std::endl;
            // }

        private:
            uint8_t m_filled = 0;
            std::string_view m_byr; // Birth Year
            std::string_view m_cid; // Country ID
            std::string_view m_ecl; // Eye Color
            std::string_view m_eyr; // Expiration Year
            std::string_view m_hcl; // Hair Color
            std::string_view m_hgt; // Height
            std::string_view m_iyr; // Issue Year
            std::string_view m_pid; // Passport ID
    };


    // Original "splitSVPtr", source: https://www.bfilipek.com/2018/07/string-view-perf-followup.html
    std::vector<std::string_view> split(std::string_view str, std::string_view delims = " ")
    {
        std::vector<std::string_view> output;

        for (auto first = str.data(), second = str.data(), last = first + str.size();
            second != last && first != last;
            first = second + 1)
        {
            second = std::find_first_of(first, last, std::cbegin(delims), std::cend(delims));

            if (first != second)
            {
                output.emplace_back(first, second - first);
            }
        }

        return output;
    }
}

void day04(const input_t& input)
{
    std::vector<passport_t> passports;
    passports.reserve(1000);

    // push first
    passports.push_back(passport_t{});
    size_t pass_idx = 0;

    parse_input(input, [&](const std::string_view& line)
    {
        if (line == "")
        {
            passports.push_back(passport_t{});
            ++pass_idx;
        }
        else
        {
            passport_t& pass = passports[pass_idx];
            const auto& fields = split(line, " ");
            for (const auto& field: fields)
            {
                const auto& keyval = split(field, ":");

                assert(keyval.size() == 2);
                const std::string_view& key = keyval[0];
                const std::string_view& val = keyval[1];

                if (key == "byr") {  pass.set_byr(val);}
                else if (key == "iyr") { pass.set_iyr(val); }
                else if (key == "eyr") { pass.set_eyr(val); }
                else if (key == "hgt") { pass.set_hgt(val); }
                else if (key == "hcl") { pass.set_hcl(val); }
                else if (key == "ecl") { pass.set_ecl(val); }
                else if (key == "pid") { pass.set_pid(val); }
                else if (key == "cid") { pass.set_cid(val); }
                else { assert(false); }
            }
        }
    });

    size_t valid_nb_pt1 = 0;
    size_t valid_nb_pt2 = 0;

    for (const auto& pass: passports)
    {
        if (pass.is_valid())
            ++valid_nb_pt1;
    }

    printf("part1: %zu, part2: %zu\n", valid_nb_pt1, valid_nb_pt2);
    assert(valid_nb_pt1 == 237);
}
