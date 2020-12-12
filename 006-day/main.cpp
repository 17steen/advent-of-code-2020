#include <ranges>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <string_view>
#include <array>
#include <algorithm>
#include <regex>
#include <charconv>
#include <boost/tokenizer.hpp>
#include <memory>

#include <set>

#include "../includes/helpers.hpp"

namespace rg = std::ranges;
namespace vw = std::views;

/*
void *operator new(size_t sz)
{
    void *m = malloc(sz);
    std::cerr << "allocation 🤮🤮🤢" << std::endl;

    return m;
}
*/

auto solve(const std::string &path)
    -> std::pair<std::size_t, std::size_t>
{
    auto file = iter_file(path, std::ios::in);

    std::array<bool, 26> yesses = {};

    std::array<int, 26> yes_count = {};

    std::size_t total = 0;

    std::size_t total_2 = 0;

    std::size_t group_size = 0;

    for (std::string_view line : file)
    {
        if (line.length() == 0)
        {
            //std::cerr << group_size << std::endl;
            total += rg::count(yesses, true);
            if (group_size)
                total_2 += rg::count(yes_count, group_size);

            rg::fill(yesses, false);
            rg::fill(yes_count, 0);

            group_size = 0;
        }
        else
        {
            ++group_size;
        }

        for (auto &letter : line)
        {
            yesses[letter - 'a'] = true;

            yes_count[letter - 'a'] += 1;
        }
    }
    total += rg::count(yesses, true);
    if (group_size)
        total_2 += rg::count(yes_count, group_size);

    return {total, total_2};
}

auto main(int, char **)
    -> int
{

    auto [part1, part2] = solve("input");

    std::cout << "both part benchmark : " << benchmark(solve, "input") << "[µs]." << std::endl;

    std::cout << "part 1 : " << part1 << '\n'
              << "part 2 : " << part2 << std::endl;

    return 0;
}