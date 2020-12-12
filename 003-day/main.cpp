#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <charconv>
#include <chrono>
#include <string_view>
#include <functional>
#include <concepts>
#include <future>
#include <mutex>
#include <execution>
#include "main.h"

auto solve_1(std::vector<std::string> &input)
    -> std::size_t
{
    constexpr char cmp = '#';
    std::size_t count = 0;
    for (int i = 0; auto &line : input)
    {
        int move = i * 3;
        if (line[move % line.size()] == cmp)
        {
            ++count;
        }
        ++i;
    }

    return count;
}

auto solve_2(const std::vector<std::string> &input, const std::vector<std::pair<int, int>> &slopes)
    -> std::size_t
{
    constexpr char cmp = '#';
    std::size_t total = 1;
    for (const auto &[right_n, down_n] : slopes)
    {
        std::size_t count = 0;
        for (std::size_t down = 0, right = 0; down < input.size(); down += down_n, right += right_n)
        {
            const auto &line = input[down];
            int move = (right) % line.size();
            if (line[move] == cmp)
            {
                ++count;
            }
        }
        total *= count;
    }
    return total;
}

auto solve_2_input(const std::string &path, const std::vector<std::pair<int, int>> &)
    -> std::size_t
{
    auto out = std::ofstream("test");
    auto file = std::ifstream(path);
    constexpr char cmp = '#';
    std::size_t count = 0;
    for (int i = 0; auto &line : into_lines(file))
    {
        int move = i * 3;
        if (line[move % line.size()] == cmp)
        {
            ++count;
        }
        ++i;
    }

    return count;
}

auto main(int, char **)
    -> int
{
    std::cout << '\n';
    auto input = read_input("./input");

    auto slopes = std::vector<std::pair<int, int>>{{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};

    std::cout << "setup time avg : " << benchmark(read_input, "./input") << "[ns]." << std::endl;

    std::cout << "part one : " << solve_2(input, {{3, 1}}) << std::endl;

    std::cout << "part two : " << solve_2(input, slopes) << std::endl;

    std::cout << solve_2_input("./input", slopes) << std::endl;

    std::cout << "time all in one B) : " << benchmark(solve_2_input, "./input", slopes) << "[ns]." << std::endl;

    std::cout << "Time : " << benchmark(solve_2, input, slopes) << "[ns]." << std::endl;

    std::cout << std::endl;

    return EXIT_SUCCESS;
}

auto read_input(const std::string &path)
    -> std::vector<std::string>
{
    auto report_file = std::ifstream(path);
    auto tmp = std::string();
    auto result = std::vector<std::string>();
    while (std::getline(report_file, tmp))
    {
        result.emplace_back(std::move(tmp));
    }
    return result;
}