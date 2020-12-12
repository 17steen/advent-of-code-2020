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

bool is_there_sum(const std::vector<int64_t> &vec, std::size_t i, std::size_t preamble, int64_t val)
{
    std::vector<int64_t> tmp;

    std::unique_copy(vec.begin() + i, vec.begin() + i + preamble, std::back_inserter(tmp));
    std::sort(tmp.begin(), tmp.end());
    for (auto left = tmp.begin(), right = tmp.end() - 1; left < right;)
    {
        int64_t sum = *left + *right;
        if (sum > val)
            --right;
        else if (sum < val)
            ++left;
        else
            return true;
    }
    return false;
}

auto contiguous_sum(const std::vector<int64_t> &vec, int64_t val)
    -> int64_t
{
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        int64_t sum = 0;
        for (std::size_t j = i; j < vec.size(); ++j)
        {
            sum += vec[j];
            if (sum > val)
            {
                break;
            }
            else if (sum == val and j - i != 0)
            {
                const auto [min, max] = std::minmax_element(vec.begin() + i, vec.begin() + j + 1);
                return *min + *max;
            }
        }
    }
    return -1;
}

auto solve(const std::string &path)
    -> std::pair<std::size_t, std::size_t>
{
    std::size_t part_1 = 0, part_2 = 0;
    std::vector<int64_t> input;

    constexpr std::size_t preamble = 25;

    auto file = iter_file(path, std::ios::in);

    for (std::size_t i = 0; std::string_view line : file)
    {
        int64_t num;
        std::from_chars(&*line.begin(), &*line.end(), num);
        input.emplace_back(num);
        if (input.size() > preamble)
        {
            if (not is_there_sum(input, i, preamble, num) and not part_1)
            {
                part_1 = num;
            }
            ++i;
        }
    }

    part_2 = contiguous_sum(input, part_1);

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