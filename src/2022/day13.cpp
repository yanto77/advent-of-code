#include "setup/all.h"
#include <stack>

ADVENT_DAY(2022, 13, 4734, 21836);

constexpr bool PARSE_DEBUG = false;

namespace 
{
    // graph node, either list (with children) or value (no children)
    struct node_t 
    {
        int32_t idx; // positive: list idx, -1: value
        union 
        {
            uint32_t count; // number of values from `idx`
            uint32_t value; // value
        };

        bool is_value() const { return idx == -1; }
    };

    struct graph_t 
    {
        str_view input;
        std::vector<node_t> data; // nodes are inline, depth-first order

        graph_t(str_view line)
            : input(line)
        {
            parse(line);
        }
        
        bool operator<(const graph_t& other) const
        {
            return compare_values(*this, 0, other, 0) == order::RIGHT;
        }

    private:
        enum class order { RIGHT, WRONG, INDECISIVE };

        void parse(str_view line)
        {
            std::vector<size_t> parentIdx; // current ancestor indices from `elems`

            size_t i = 0;
            while (i < line.size())
            {
                if (line[i] == '[')
                {
                    int32_t nextIdx = (int32_t)(data.size() + 1);
                    node_t sublist { .idx = nextIdx, .count = 0 };
                    data.push_back(sublist);
                    parentIdx.push_back(data.size() - 1);
                }
                else if (line[i] == ']')
                {
                    parentIdx.pop_back();
                    if (!parentIdx.empty())
                    {
                        data[parentIdx.back()].count += 1;
                    }
                }
                else if (line[i] == ',' || line[i] == '\n')
                {
                }
                else // number
                {
                    uint32_t num1 = parse_uint(line[i]);
                    if ('0' <= line[i+1] && line[i+1] <= '9') // double digit
                    {
                        uint32_t num2 = parse_uint(line[i+1]);
                        data.push_back({ .idx = -1, .value = num1 * 10 + num2 });
                    }
                    else // single digit
                    {
                        data.push_back({ .idx = -1, .value = num1 });
                    }

                    if (!parentIdx.empty())
                    {
                        data[parentIdx.back()].count += 1;
                    }
                }
                i++;
            }

            if constexpr (PARSE_DEBUG)
            {
                fmt::print("> parsed: "); 
                print();
            }
        }

        static order compare_values(const graph_t& lgraph, int32_t lidx, const graph_t& rgraph, int32_t ridx)
        {
            node_t lelem = lgraph.data[lidx];
            node_t relem = rgraph.data[ridx];

            if (lelem.is_value() && relem.is_value())
            {
                if (lelem.value < relem.value) return order::RIGHT;
                if (lelem.value > relem.value) return order::WRONG;
                return order::INDECISIVE;
            }
            else 
            {
                node_t llist = (!lelem.is_value()) ? lelem : node_t { .idx = lidx, .count = 1 };
                node_t rlist = (!relem.is_value()) ? relem : node_t { .idx = ridx, .count = 1 };

                int32_t lskip = 0; // when subnode is a list, we will need to skip 
                int32_t rskip = 0; // extra elements (which are part of the sublist)

                for (int32_t i = 0; i < max(llist.count, rlist.count); i++)
                {
                    if (i >= llist.count && i < rlist.count) // left ran out of items
                    {
                        return order::RIGHT;
                    }
                    else if (i < llist.count && i >= rlist.count) // right ran out of items
                    {
                        return order::WRONG;
                    }
                    else 
                    {
                        int32_t lsubidx = llist.idx + i + lskip;
                        int32_t rsubidx = rlist.idx + i + rskip;

                        order res = compare_values(lgraph, lsubidx, rgraph, rsubidx);
                        if (res != order::INDECISIVE)
                        {
                            return res;
                        }

                        node_t lsubelem = lgraph.data[lsubidx];
                        node_t rsubelem = rgraph.data[rsubidx];
                        if (!lsubelem.is_value()) lskip += lsubelem.count;
                        if (!rsubelem.is_value()) rskip += rsubelem.count;
                    }
                }

                return order::INDECISIVE;
            }
        }
    
        [[maybe_unused]]
        void print()
        {
            fmt::print("[");
            for (size_t i = 0; i < data.size(); i++)
            {   
                node_t e = data[i];
                if (e.idx == -1) 
                    fmt::print("{}", e.value);
                else
                    fmt::print("list({}, {})", e.idx, e.count);

                if (i != (data.size() - 1)) 
                    fmt::print(", ");
            }
            fmt::print("]\n");
        }
    };
}

result_t Day_2022_13::run_solution(str_view input) const
{
    std::vector<graph_t> packets;
    for_each_split(input, '\n', [&](str_view line)
    {
        if (!line.empty())
        {
            packets.push_back(graph_t(line));
        }
    });

    size_t part1 = 0;
    for (size_t i = 0; i < packets.size(); i += 2)
    {
        if (packets[i] < packets[i+1])
        {
            part1 += (i / 2) + 1;
        }
    }

    packets.push_back(graph_t("[[2]]"));
    packets.push_back(graph_t("[[6]]"));
    std::sort(packets.begin(), packets.end());

    size_t part2 = 1;
    for (size_t i = 0; i < packets.size(); i++)
    {
        if (packets[i].input == "[[2]]" || packets[i].input == "[[6]]")
        {
            part2 *= (i + 1);
        }
    }

    return { part1, part2 };
}

void Day_2022_13::run_tests() const
{
    struct test_data_t { str_view lhs; str_view rhs; bool expected; };
    std::vector<test_data_t> data = {
        { "[1,1,3,1,1]", "[1,1,5,1,1]", true },
        { "[[1],[2,3,4]]", "[[1],4]", true },
        { "[9]", "[[8,7,6]]", false },
        { "[[4,4],4,4]", "[[4,4],4,4,4]", true },
        { "[7,7,7,7]", "[7,7,7]", false },
        { "[]", "[3]", true },
        { "[[[]]]", "[[]]", false },
        { "[1,[2,[3,[4,[5,6,7]]]],8,9]", "[1,[2,[3,[4,[5,6,0]]]],8,9]", false }
    };

    size_t test_part = 0;
    for (size_t i = 0; i < data.size(); i++)
    {
        const auto& [lhs, rhs, expected] = data[i];
        graph_t lhs_graph(lhs);
        graph_t rhs_graph(rhs);

        bool actual = (lhs_graph < rhs_graph);

        if constexpr (PARSE_DEBUG)
        {
            const char* status = (actual != expected) ? COLOR_RED() "ERR" COLOR_RESET() : COLOR_GREEN() "OK" COLOR_RESET();
            fmt::print("ret: {}, exp: {}, {}\n\n\n", actual, expected, status);
        }

        if (actual)
        {
            test_part += (i + 1);
        }
    }

    assert(test_part == 13);
}
