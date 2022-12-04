#include "setup/all.h"

ADVENT_DAY(2020, 21, 2307, 1);

namespace
{
    struct ingredient_t
    {
        enum allergen_t : uint8_t
        {
            DAIRY,
            EGGS,
            FISH,
            NUTS,
            PEANUTS,
            SHELLFISH,
            SOY,
            WHEAT,
        };
        std::array<uint64_t, 8> allergens {};
        size_t count { 1 };

        void sum(const std::vector<str_view>& input)
        {
            for (const auto& a : input)
            {
                if (a == "dairy") ++allergens[DAIRY];
                else if (a == "eggs") ++allergens[EGGS];
                else if (a == "peanuts") ++allergens[PEANUTS];
                else if (a == "nuts") ++allergens[NUTS];
                else if (a == "shellfish") ++allergens[SHELLFISH];
                else if (a == "wheat") ++allergens[WHEAT];
                else if (a == "fish") ++allergens[FISH];
                else if (a == "soy") ++allergens[SOY];

                // fmt::print(" - added to: {}\n", a)
                // fmt::print("  -> result: {}\n", allergens)
            }
        }
    };

    std::array<str_view, 8> decide_allergens(std::unordered_map<str_view, ingredient_t> ingr_map)
    {
        std::array<str_view, 8> allergens;

        while (true)
        {
            std::array<uint64_t, 8> max_values {};
            std::array<str_view, 8> max_value_names {};
            for (const auto& [name, ingredient] : ingr_map)
            {
                for (size_t idx = 0; idx < 8; ++idx)
                {
                    auto count = ingredient.allergens[idx];
                    if (count > max_values[idx])
                    {
                        max_values[idx] = count;
                        max_value_names[idx] = name;
                    }
                    else if (count == max_values[idx] && max_value_names[idx] != "")
                    {
                        max_value_names[idx] = ""; // invalidate
                    }
                }
            }

            bool at_least_one = false;
            for (size_t idx = 0; idx < 8; ++idx)
            {
                const auto& name = max_value_names[idx];
                if (name != "")
                {
                    allergens[idx] = name;
                    ingr_map[name].allergens = {}; // invalidate
                    at_least_one = true;
                }
            }

            if (!at_least_one)
                break;

            for (auto& [name, ingredient] : ingr_map)
            {
                for (size_t idx = 0; idx < 8; ++idx)
                    if (max_value_names[idx] != "")
                        ingredient.allergens[idx] = 0;
            }
        }

        return allergens;
    }

    std::pair<size_t, std::array<str_view, 8>> evaluate(str_view input)
    {
        std::unordered_map<str_view, ingredient_t> ingr_map;

        for_each_split(input, '\n', [&](str_view line)
        {
            // std::cout << line << std::endl;

            auto [part1, part2] = split_at(line, '(');
            part2.remove_prefix(sizeof("contains ") - 1);
            part2.remove_suffix(sizeof(")") - 1);

            const auto& ingredients = split(part1, ' ');
            const auto& allergens = split(part2, ", ");

            // fmt::print(" - ingr: {}\n, ingredients);
            // fmt::print(" - allerg: {}\n, allergens);

            for (str_view ingr : ingredients)
            {
                // printf("- adding to: "); std::cout << ingr << std::endl;
                auto it = ingr_map.find(ingr);
                if (it != ingr_map.end())
                {
                    it->second.sum(allergens);
                    ++it->second.count;
                }
                else
                {
                    auto [it2, inserted] = ingr_map.emplace(ingr, ingredient_t {});
                    it2->second.sum(allergens);
                }
            }
        });

        const auto& allergens = decide_allergens(ingr_map);

        size_t part1 = 0;
        for (const auto& [name, value] : ingr_map)
        {
            if (get_index_of(allergens, name) == -1)
            {
                part1 += value.count;
            }
        }

        return { part1, allergens };
    }
}

result_t Day_2020_21::run_solution(str_view input) const
{
    const auto& [part1, part2] = evaluate(input);

    // TODO: add this into main.cpp validation?
    std::array<str_view, 8> expected { "cljf", "frtfg", "vvfjj", "qmrps", "hvnkk", "qnvx", "cpxmpc", "qsjszn" };
    assert(part2 == expected);

    return { part1, 1 };
}

void Day_2020_21::run_tests() const
{
    char text1[] = "mxmxvkd kfcds sqjhc nhms (contains dairy, fish)\n"
                   "trh fvjkl sbzzf mxmxvkd (contains dairy)\n"
                   "sqjhc fvjkl (contains soy)\n"
                   "sqjhc mxmxvkd sbzzf (contains fish)\n";
    const auto& [part1, part2] = evaluate(text1);
    assert(part1 == 5);
}
