#include <ranges>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <string_view>
#include <array>
#include <algorithm>
#include <regex>
#include <charconv>
#include <boost/tokenizer.hpp>
#include <memory>

#include <cstdio>
#include <unordered_set>

#include "../includes/helpers.hpp"

#define time_start(x) auto x = std::chrono::high_resolution_clock::now();

#define time_end(x, str) std::cout << (str) << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (x)).count() << "[µs].\n";

namespace rg = std::ranges;
namespace vw = std::views;

auto count_paths(const std::vector<int>::iterator &begin, const std::vector<int>::iterator &end,
                 std::unordered_map<std::vector<int>::iterator::difference_type, std::size_t> &map)
    -> std::size_t
{
    std::size_t count = 1;
    for (auto base = begin; base != end - 1; ++base)
    {
        auto other = base + 1;

        std::size_t iter = 0;

        //find all possible adaptors
        while (other != end && (*other - *base) <= 3)
        {
            if (iter >= 1) // if there is more than one,
                           // calculate all possibilities
            {
                auto dist = std::distance(other, end);
                if (auto res = map.find(dist); res != map.end())
                {
                    count += res->second;
                }
                else
                {
                    std::size_t tmp = count_paths(other, end, map);
                    map.emplace(dist, tmp);
                    count += tmp;
                }
            }
            ++other;
            ++iter;
        }
    }
    return count;
}

auto solve(const std::string &path)
    -> std::pair<std::size_t, std::size_t>
{
    std::size_t part_1 = 0, part_2 = 0;

    std::array<int, 3> counts{};

    auto file = iter_file(path, std::ios::in);

    std::vector<int> adapters{};

    adapters.emplace_back(0);

    for (std::string_view str : file)
    {
        int num;
        std::from_chars(&*str.begin(), &*str.end(), num);
        adapters.push_back(num);
    }

    adapters.emplace_back(*std::max_element(adapters.begin(), adapters.end()) + 3);

    std::sort(adapters.begin(), adapters.end());

    for (auto it = adapters.begin(); it != adapters.end() - 1; ++it)
    {
        auto next = it + 1;
        int diff = *next - *it;
        if (diff <= 3 && diff > 0) // part1;
            ++counts[diff - 1];
    }

    std::unordered_map<std::vector<int>::iterator::difference_type, std::size_t> map{};
    part_2 = count_paths(adapters.begin(), adapters.end(), map);

    part_1 = counts[0] * counts[2];

    return {part_1, part_2};
}

auto main(int, char **)
    -> int
{

    time_start(solving);
    auto [part1, part2] = solve("input");

    //std::cout << "both part benchmark : " << benchmark(solve, "input") << "[µs]." << std::endl;

    std::cout << "part 1 : " << part1 << '\n'
              << "part 2 : " << part2 << std::endl;
    time_end(solving, "execution time : ");

    return 0;
}
