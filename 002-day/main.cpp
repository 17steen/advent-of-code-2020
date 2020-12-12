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
#include <regex>

#include "main.h"

auto solve_1(const std::vector<std::string> &input)
    -> int
{
    int start;
    int end;
    char c;

    int count = 0;

    for (const auto &pwd : input)
    {
        auto before = pwd.begin();
        auto where = std::find_if(before, pwd.end(), [](const auto &ch) { return ch > '9' || ch < '0'; });
        std::from_chars(pwd.data(), where.base(), start);
        before = where + 1;
        where = std::find_if(before, pwd.end(), [](const auto &ch) { return ch > '9' || ch < '0'; });
        std::from_chars(before.base(), where.base(), end);
        c = *(where += 1);
        where += 2;
        auto amount = std::count(where, pwd.end(), c);

        if (amount >= start && amount <= end)
        {
            ++count;
        }
    }
    return count;
}

auto solve_2_multithread(const std::vector<std::string> &vec)
    -> int
{
    int count = 0;
    auto count_mutex = std::mutex();

    char c;
    size_t start = 0;
    size_t end = 0;

    std::for_each(std::execution::par_unseq, vec.begin(), vec.end(), [&](const std::string &pwd) {
        auto before = pwd.begin();
        auto where = std::find_if(before, pwd.end(), [](const auto &ch) { return ch > '9' or ch < '0'; });
        std::from_chars(pwd.data(), where.base(), start);
        before = where + 1;
        where = std::find_if(before, pwd.end(), [](const auto &ch) { return ch > '9' or ch < '0'; });
        std::from_chars(before.base(), where.base(), end);
        c = *(where += 1);
        where += 3;
        auto password = std::string_view(where, pwd.end());

        --start;
        --end;

        auto matches = int(0);

        if (start < password.size() and password[start] == c)
        {
            ++matches;
        }
        if (end < password.size() and password[end] == c)
        {
            ++matches;
        }
        if (matches == 1)
        {
            //const auto lock = std::lock_guard<std::mutex>(count_mutex);
            ++count;
        }
    });
    return count;
}

auto solve_2(const std::vector<std::string> &input)
    -> int
{
    //auto file = std::ofstream("copy");

    int count = 0;

    for (auto it = input.begin(); it != input.end(); ++it)
    {
        const auto &pwd = *it;
        size_t start = 0;
        size_t end = 0;
        char c;

        auto before = pwd.begin();
        auto where = std::find_if(before, pwd.end(), [](const auto &ch) { return ch > '9' or ch < '0'; });
        std::from_chars(pwd.data(), where.base(), start);
        before = where + 1;
        where = std::find_if(before, pwd.end(), [](const auto &ch) { return ch > '9' or ch < '0'; });
        std::from_chars(before.base(), where.base(), end);
        c = *(where += 1);
        where += 3;
        auto password = std::string_view(where, pwd.end());

        //debug
        //file << start << '-' << end << ' ' << c << ": " << password << '\n';

        --start;
        --end;

        auto matches = int(0);

        if (start < password.size() and password[start] == c)
        {
            ++matches;
        }
        if (end < password.size() and password[end] == c)
        {
            ++matches;
        }
        if (matches == 1)
        {
            ++count;
        }
    }
    return count;
}

auto solve_2_scanf(const std::vector<std::string> &input)
    -> int
{
    size_t start, end;
    char c;
    char buff[255] = {0};

    auto size = sizeof(buff);

    int count = 0;

    for (const auto &pwd : input)
    {

        std::sscanf(pwd.data(), "%lu-%lu %c: %s\n", &start, &end, &c, buff);
        --start;
        --end;

        int matches = 0;

        if (start < size and buff[start] == c)
        {
            ++matches;
        }
        if (end < size and buff[end] == c)
        {
            ++matches;
        }
        if (matches == 1)
        {
            ++count;
        }
    }
    return count;
}

template <typename F, typename... Args>
double benchmark(F func, Args &&... args)
{
    int amount = 1000;
    double sum = 0;
    for (int i = 0; i < amount; ++i)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto t2 = std::chrono::high_resolution_clock::now();
        sum += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }
    return sum / amount;
}

auto with_regex(const std::vector<std::string> &input)
    -> std::size_t
{
    std::size_t count = 0;
    auto reg = std::regex("^(\\d+)\\-(\\d+) ([a-z]): ([a-z]+)$");
    for (const auto &str : input)
    {
        std::array<std::string, 4> arr;
        std::smatch res;
        if (std::regex_match(str, res, reg))
        {
            std::ranges::copy(res | std::views::drop(1), arr.begin());

            auto [first_str, last_str, ch_str, pwd] = arr;
            int first, last;
            std::from_chars(first_str.data(), first_str.data() + first_str.size(), first);
            std::from_chars(last_str.data(), last_str.data() + last_str.size(), last);
            char ch = ch_str[0];

            auto amount = std::ranges::count(pwd, ch);

            if (amount >= first && amount <= last)
            {
                ++count;
            }
        }
        else
        {
            std::cerr << "WTF" << std::endl;
            std::cerr << str << std::endl;
        }
    }

    return count;
}

auto main(int, char **)
    -> int
{
    auto input = read_input("./input");

    std::cout << '\n';

    std::cout << with_regex(input) << "    " << solve_1(input) << std::endl;
    std::cout << benchmark(with_regex, input) << "[µs] with rgx, " << benchmark(solve_1, input) << "[µs] without." << std::endl;

    std::cout << solve_2_multithread(input) << std::endl;

    //std::cout << "setup benchmark : " << benchmark(read_input, "./input") << std::endl;

    std::cout << "without parallelism : " << benchmark(solve_2, input) << std::endl;

    std::cout << "with parallelism : " << benchmark(solve_2_multithread, input) << std::endl;

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