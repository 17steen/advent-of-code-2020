#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <charconv>
#include <chrono>

#include "main.h"

auto solve_1(const std::vector<int> report)
    -> int
{
    for (size_t i = 0; i < report.size(); ++i)
    {
        for (size_t j = i; j < report.size(); ++j)
        {
            if (report[i] + report[j] == 2020)
                return report[i] * report[j];
        }
    }
    return 0;
}

auto solve_2(const std::vector<int> report)
    -> int
{
    for (size_t i = 0; i < report.size(); ++i)
    {
        for (size_t j = i; j < report.size(); ++j)
        {
            for (size_t k = j; k < report.size(); ++k)
                if (report[i] + report[j] + report[k] == 2020)
                    return report[i] * report[j] * report[k];
        }
    }
    return 0;
}

auto main(int, char **)
    -> int
{
    auto report = read_report("./input");

    //std::for_each(report.begin(), report.end(), [](auto &str) { std::cout << str << std::endl; });

    //std::cout << "\n\n\n";
    std::cout << solve_1(report);

    std::cout << '\n';

    auto begin = std::chrono::steady_clock::now();

    solve_2(report);

    auto end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << std::endl;

    return EXIT_SUCCESS;
}

auto read_report(const std::string &path)
    -> std::vector<int>
{
    auto report_file = std::ifstream(path);
    auto tmp = std::string();
    auto result = std::vector<int>();
    while (std::getline(report_file, tmp))
    {
        int integer;
        std::from_chars(tmp.data(), tmp.data() + tmp.size(), integer);
        result.emplace_back(integer);
    }
    return result;
}