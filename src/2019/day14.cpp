#include "setup/all.h"

ADVENT_DAY(2019, 14, 0, 0);

namespace
{
    struct recipe_t
    {
        size_t in_id;
        size_t in_cost;
        size_t out_cost;
        size_t out_id;
    };

    typedef std::unordered_map<size_t, std::vector<recipe_t>> recipe_list_t;

    std::unordered_map<size_t, std::string> debug_map;

    // returns [output_id, output_num]
    std::pair<size_t, size_t> parse_num_type(str_view line)
    {
        const auto& [num, type] = split_single(line, " ");
        // fmt::print("  -> num: [{}] type: [{} = {}]\n", num, fnv1a_hash(type), type);
        auto id = fnv1a_hash(type);
        debug_map[id] = type;
        return { id, to_int<size_t>(num) };
    }

    [[maybe_unused]]
    recipe_list_t parse(str_view input)
    {
        recipe_list_t recipes;

        parse_input(input, [&](str_view line)
        {
            const auto& [ins, out] = split_single(line, " => ");

            const auto& [out_id, out_num] = parse_num_type(out);
            recipes[out_id] = {};

            for (const auto& in: split_multi(ins, ", "))
            {
                const auto& [in_id, in_num] = parse_num_type(in);
                recipes[out_id].push_back(recipe_t{
                    .in_id = in_id,
                    .in_cost = in_num,
                    .out_cost = out_num,
                    .out_id = out_id,
                });
            }
        });

        return recipes;
    }

    [[maybe_unused]]
    size_t compute_ore_count(const recipe_list_t& recipes)
    {
        // answer the question "what can the FUEL be broken down into?"
        std::unordered_map<size_t, size_t> inventory; // map {type id => available count}
        inventory[fnv1a_hash("FUEL")] = 1;

        auto apply_recipe = [&](size_t part_id, const std::vector<recipe_t>& recipe_list) -> void
        {
            const size_t part_count = inventory[part_id];
            const size_t recipe_count = part_count / recipe_list[0].out_cost;

            if (recipe_count == 0)
            {
                // if (inventory.size() == 2) // make one extra batch
                //     recipe_count = 1;
                // else // don't have enough ingredients, but they may still come from other recipes
                // if (add_extra_batch)
                // {
                //     fmt::print(" -> add +1 for next (inv size: {})\n", inventory.size());
                //     recipe_count = 1;
                // }
                // else
                return;
            }

            const size_t removed_nb = recipe_count * recipe_list[0].out_cost;
            fmt::print(" - can use {} (have {}) for {} recipe instances\n", debug_map[part_id], part_count, recipe_count);
            // fmt::print("   - (inv: {})\n", inventory);
            fmt::print("   - del {} of {}\n", removed_nb, debug_map[part_id]);

            // assert(part_count >= removed_nb);
            inventory[part_id] -= std::min(inventory[part_id], removed_nb);

            if (inventory[part_id] == 0)
            {
                fmt::print("     - removing {} from inventory\n", debug_map[part_id]);
                inventory.erase(part_id);
            }

            for (const auto& part: recipe_list)
            {
                const size_t added_nb = recipe_count * part.in_cost;
                fmt::print("   - add {} of {}\n", added_nb, debug_map[part.in_id]);
                inventory[part.in_id] += added_nb;
            }
        };

        // returns true if was able to decompose something, otherwise false.
        auto decompose = [&]() -> bool
        {
            for (const auto& [part_id, part_count]: inventory)
            {
                assert(part_count != 0);

                auto it = recipes.find(part_id);
                if (it != recipes.end())
                {
                    apply_recipe(it->first, it->second);
                    return true;
                }
            }

            return false;
        };

        std::queue<size_t> unvisited;
        unvisited.push(fnv1a_hash("FUEL"));

        while (!unvisited.empty())
        {
            bool decomposed_smth = decompose();
            if (!decomposed_smth)
                break;
        }

        fmt::print("inventory at the end:\n");
        for (const auto& [part_id, part_count]: inventory)
        {
            fmt::print(" - {}, num={}\n", debug_map[part_id], part_count);
        }
        fmt::print("\n\n");
        return inventory[fnv1a_hash("ORE")];
    }
}

output_t Day_2019_14::run_solution(str_view /* input */) const
{
    size_t part1 = 0, part2 = 0;
    return {part1, part2};
}

void Day_2019_14::run_tests() const
{
    // char text1[] = ""
    //     "10 ORE => 10 A\n"
    //     "1 ORE => 1 B\n"
    //     "7 A, 1 B => 1 C\n"
    //     "7 A, 1 C => 1 D\n"
    //     "7 A, 1 D => 1 E\n"
    //     "7 A, 1 E => 1 FUEL\n";
    // const auto& recipes1 = parse(input_t { text1, sizeof(text1) });
    // assert(31 == compute_ore_count(recipes1));

    // char text2[] = ""
    //     "9 ORE => 2 A\n"
    //     "8 ORE => 3 B\n"
    //     "7 ORE => 5 C\n"
    //     "3 A, 4 B => 1 AB\n"
    //     "5 B, 7 C => 1 BC\n"
    //     "4 C, 1 A => 1 CA\n"
    //     "2 AB, 3 BC, 4 CA => 1 FUEL\n";
    // const auto& recipes2 = parse(input_t { text2, sizeof(text2) });
    // assert(165 == compute_ore_count(recipes2));

    // char text3[] = ""
    //     "157 ORE => 5 NZVS\n"
    //     "165 ORE => 6 DCFZ\n"
    //     "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL\n"
    //     "12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ\n"
    //     "179 ORE => 7 PSHF\n"
    //     "177 ORE => 5 HKGWZ\n"
    //     "7 DCFZ, 7 PSHF => 2 XJWVT\n"
    //     "165 ORE => 2 GPVTF\n"
    //     "3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT\n";
    // const auto& recipes3 = parse(input_t { text3, sizeof(text3) });
    // assert(13312 == compute_ore_count(recipes3));

    // char text4[] = ""
    //     "2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG\n"
    //     "17 NVRVD, 3 JNWZP => 8 VPVL\n"
    //     "53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL\n"
    //     "22 VJHF, 37 MNCFX => 5 FWMGM\n"
    //     "139 ORE => 4 NVRVD\n"
    //     "144 ORE => 7 JNWZP\n"
    //     "5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC\n"
    //     "5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV\n"
    //     "145 ORE => 6 MNCFX\n"
    //     "1 NVRVD => 8 CXFTF\n"
    //     "1 VJHF, 6 MNCFX => 4 RFSQX\n"
    //     "176 ORE => 6 VJHF\n";
    // const auto& recipes4 = parse(input_t { text4, sizeof(text4) });
    // assert(180697 == compute_ore_count(recipes4));

    // char text5[] = ""
    //     "171 ORE => 8 CNZTR\n"
    //     "7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL\n"
    //     "114 ORE => 4 BHXH\n"
    //     "14 VRPVC => 6 BMBT\n"
    //     "6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL\n"
    //     "6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT\n"
    //     "15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW\n"
    //     "13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW\n"
    //     "5 BMBT => 4 WPTQ\n"
    //     "189 ORE => 9 KTJDG\n"
    //     "1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP\n"
    //     "12 VRPVC, 27 CNZTR => 2 XDBXC\n"
    //     "15 KTJDG, 12 BHXH => 5 XCVML\n"
    //     "3 BHXH, 2 VRPVC => 7 MZWV\n"
    //     "121 ORE => 7 VRPVC\n"
    //     "7 XCVML => 6 RJRHP\n"
    //     "5 BHXH, 4 VRPVC => 5 LTCX\n";
    // const auto& recipes5 = parse(input_t { text5, sizeof(text5) });
    // assert(2210736 == compute_ore_count(recipes5));
}
