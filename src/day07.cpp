#include "advent2020.h"

#include <execution>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>

#include <3rd-party/ctre.hpp>

typedef std::unordered_map<sv, size_t> key_dict_t;
struct link_t
{
    size_t from;
    size_t to;
    int cost;
};
struct result_t
{
    key_dict_t key_dict;
    std::vector<link_t> graph;
};

namespace
{
    std::pair<uint8_t, sv> parse_single_bag(const sv& text)
    {
        static constexpr auto pattern = ctll::fixed_string { "\\s?(no|[0-9]+) ([a-z ]+) bags?\\.?$" };
        auto m = ctre::match<pattern>(text);

        if (m.get<1>().to_view() == "no")
        {
            return { 0, "" };
        }
        else
        {
            return 
            { 
                to_int<uint8_t>(m.get<1>().to_view()),
                m.get<2>().to_view() 
            };
        }

        // TODO: regexing seems to take bunch of time, so what about just going by whitespaces? using e.g. parse_multi()
    }

    result_t parse_input(const input_t& input)
    {
        result_t result;
        result.key_dict.reserve(1000);

        // 1. Map with increasing index for every new bag color.
        size_t idx = -1;
        auto get_id = [&](const sv& key) -> size_t
        {
            auto it = result.key_dict.find(key);
            if (it != result.key_dict.end())
            {
                return it->second;
            }
            else
            {
                result.key_dict.insert({ key, ++idx });
                return idx;
            }
        };

        // 2. Parse the strings, for example:
        //   - given: "muted plum bags contain 3 shiny brown bags, 4 shiny teal bags."
        //   - saves: MP => {{3, SB}, {4, ST}} (where MP, SB, and ST are indexes)
        parse_input(input, [&](const sv& line) 
        {
            auto [node_from, value] = split_single(line, " bags contain ");
            const auto from_id = get_id(node_from);

            value.remove_suffix(1); // remove last period

            for (const sv& part : split_multi(value, ", "))
            {
                const auto& [cost, node_to] = parse_single_bag(part);
                if (cost != 0)
                {
                    result.graph.emplace_back(link_t { 
                        .from = from_id, 
                        .to = get_id(node_to), 
                        .cost = cost
                    });
                }
            }
        });

        return result;
    }

    size_t get_links_to(result_t& res, const sv& node)
    {
        std::set<size_t> links_to_node;
        std::queue<size_t> node_queue;
        node_queue.push((*res.key_dict.find(node)).second);

        while (!node_queue.empty())
        {
            size_t node_to = node_queue.front();

            for (const link_t& link : res.graph)
            {
                if (link.to == node_to)
                {
                    links_to_node.insert(link.from);
                    node_queue.push(link.from);
                }
            }

            node_queue.pop();
        }

        // TODO: evaluates visited nodes multiple times.
        return links_to_node.size();
    }

    size_t get_contained_nodes(result_t& res, const sv& node)
    {
        std::vector<std::pair<size_t, uint>> contained;
        std::queue<std::pair<size_t, uint>> node_queue;
        node_queue.push({ (*res.key_dict.find(node)).second, 1 });

        while (!node_queue.empty())
        {
            const auto& [node_from, prev_cost] = node_queue.front();

            for (const link_t& link : res.graph)
            {
                if (link.from == node_from)
                {
                    uint full_cost = prev_cost * link.cost;
                    contained.push_back({ link.to, full_cost });
                    node_queue.push({ link.to, full_cost });
                }
            }

            node_queue.pop();
        }

        size_t result = 0;
        for (auto& n : contained)
            result += n.second;
        return result;
    }
}

output_t day07(const input_t& input)
{
    result_t res = parse_input(input);

    const size_t con_nb = get_links_to(res, "shiny gold");
    const size_t con_cost = get_contained_nodes(res, "shiny gold");
    return {con_nb, con_cost};
}

void day07_test()
{
    {
        auto asd1 = parse_single_bag("4 striped lavender bags");
        assert(asd1.first == 4);
        assert(asd1.second == "striped lavender");

        auto asd2 = parse_single_bag("1 clear fuchsia bag");
        assert(asd2.first == 1);
        assert(asd2.second == "clear fuchsia");

        auto asd3 = parse_single_bag("4 striped lavender bags.");
        assert(asd3.first == 4);
        assert(asd3.second == "striped lavender");

        auto asd4 = parse_single_bag("1 clear fuchsia bag.");
        assert(asd4.first == 1);
        assert(asd4.second == "clear fuchsia");

        auto asd5 = parse_single_bag("no other bags");
        assert(asd5.first == 0);
        assert(asd5.second == "");
    }

    {
        char text[513] = "light red bags contain 1 bright white bag, 2 muted yellow bags.\n"
                         "dark orange bags contain 3 bright white bags, 4 muted yellow bags.\n"
                         "bright white bags contain 1 shiny gold bag.\n"
                         "muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.\n"
                         "shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.\n"
                         "dark olive bags contain 3 faded blue bags, 4 dotted black bags.\n"
                         "vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.\n"
                         "faded blue bags contain no other bags.\n"
                         "dotted black bags contain no other bags.\n";

        auto res = parse_input(input_t { text, 513 });
        const size_t con_nb = get_links_to(res, "shiny gold");
        const size_t con_cost = get_contained_nodes(res, "shiny gold");
        assert(con_nb == 4);
        assert(con_cost == 32);
    }

    {
        char text[298] = "shiny gold bags contain 2 dark red bags.\n"
                         "dark red bags contain 2 dark orange bags.\n"
                         "dark orange bags contain 2 dark yellow bags.\n"
                         "dark yellow bags contain 2 dark green bags.\n"
                         "dark green bags contain 2 dark blue bags.\n"
                         "dark blue bags contain 2 dark violet bags.\n"
                         "dark violet bags contain no other bags.\n";
        auto res = parse_input(input_t { text, 298 });
        const size_t con_nb = get_links_to(res, "shiny gold");
        const size_t con_cost = get_contained_nodes(res, "shiny gold");
        assert(con_nb == 0);
        assert(con_cost == 126);
    }
}
