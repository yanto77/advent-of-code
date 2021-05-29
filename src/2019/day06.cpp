#include "setup/all.h"

ADVENT_DAY(2019, 6, 402879, 484);

namespace
{
    typedef std::unordered_map<uint32_t, std::vector<uint32_t>> orbits_t;

    constexpr uint32_t get_id(const char* c)
    {
        return c[0] << 16 | c[1] << 8 | c[2];
    }

    [[maybe_unused]]
    std::string get_str(uint32_t id)
    {
        return fmt::format("{}{}{}",
            static_cast<char>(id >> 16),
            static_cast<char>(id >> 8),
            static_cast<char>(id)
        );
    }

    orbits_t parse(const input_t& input)
    {
        orbits_t links;
        parse_input(input, [&](const sv& line)
        {
            uint32_t a = get_id(&line[0]);
            uint32_t b = get_id(&line[4]);

            if (links.find(a) != links.end())
                links[a].push_back(b);
            else
                links[a] = { b };
            
        });
        return links;
    }

    size_t get_orbit_checksum(const orbits_t& data)
    {
        struct visited_node_t { uint32_t id; size_t depth; };

        std::queue<visited_node_t> unvisited;
        unvisited.push({ get_id("COM"), 0 });

        size_t checksum = 0; // number of all links
        while (!unvisited.empty())
        {
            const visited_node_t& node = unvisited.front();
            unvisited.pop();
            
            checksum += node.depth;

            if (data.find(node.id) != data.end())
            {
                for (uint32_t child_node: data.at(node.id))
                {
                    unvisited.push({ child_node, node.depth + 1 });
                }
            }
        }

        return checksum;
    }

    std::vector<uint32_t> get_parents(const orbits_t& data, uint32_t node)
    {
        std::vector<uint32_t> parents;

        while (node != get_id("COM"))
        {
            auto it = std::find_if(data.begin(), data.end(), [&](const auto& kv)
            {
                const auto& children = kv.second;
                return std::find(children.begin(), children.end(), node) != children.end();
            });

            // assume we always find the parent, because we check for COM outside
            parents.push_back(it->first);
            node = it->first;
        }

        return parents;
    }

    size_t get_orbit_difference(const orbits_t& data, uint32_t a, uint32_t b)
    {
        std::vector<uint32_t> parents_a = get_parents(data, a);
        std::vector<uint32_t> parents_b = get_parents(data, b);

        for (size_t idx = 0; idx < parents_a.size() && idx < parents_b.size(); ++idx)
        {
            auto pa_idx = parents_a.size() - 1 - idx;
            auto pb_idx = parents_b.size() - 1 - idx;

            if (parents_a[pa_idx] != parents_b[pb_idx])
                return pa_idx + pb_idx + 2;
        }

        return 0;
    }
}

output_t Day_2019_6::run_solution(const input_t& input) const
{
    // {key => value}, where `value` is in orbit around `key`
    const auto& orbits = parse(input);
    size_t part1 = get_orbit_checksum(orbits);
    size_t part2 = get_orbit_difference(orbits, get_id("YOU"), get_id("SAN"));

    return {part1, part2};
}

void Day_2019_6::run_tests() const
{
    char text1[] = "COM)BBB\nBBB)CCC\nCCC)DDD\nDDD)EEE\nEEE)FFF\nBBB)GGG\n"
                   "GGG)HHH\nDDD)III\nEEE)JJJ\nJJJ)KKK\nKKK)LLL\nKKK)YOU\nIII)SAN\n";
    const auto& links = parse(input_t { text1, sizeof(text1) });
    assert(get_orbit_checksum(links) == 42 + 12); // the 12 comes from adding YOU+SAN
    assert(get_orbit_difference(links, get_id("YOU"), get_id("SAN")) == 4);
}
