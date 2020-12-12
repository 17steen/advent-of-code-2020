#include <ranges>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <array>
#include <algorithm>
#include <regex>
#include <charconv>
#include <boost/tokenizer.hpp>

#include "main.h"
#include "../includes/ctre.hpp"

namespace rg = std::ranges;
namespace vw = std::views;

/*
void *operator new(size_t sz)
{
    void *m = malloc(sz);
    std::cout << "User Defined :: Operator new" << std::endl;

    return m;
}
*/

auto check_fields(const std::array<std::string, 7> fields, const std::array<bool, 7> &contains)
    -> bool
{
    return rg::count(contains, true) == contains.size();
}

auto solve_1(const std::string &path)
    -> std::size_t
{
    const std::array<std::string, 7> fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    std::array<bool, 7> contains = {false, false, false, false, false, false, false};
    std::string line;
    std::ifstream file(path);
    std::size_t validated = 0;

    while (std::getline(file, line))
    {
        if (line.size() == 0)
        {
            validated += check_fields(fields, contains);
            rg::fill(contains, false);
        }
        else
        {
            boost::char_separator<char> sep(" ");
            for (const auto &str : boost::tokenizer(line, sep))
            {
                std::string_view field(str.data(), str.data() + 3);
                auto res = rg::find(fields, field);
                if (res != fields.end())
                    contains[std::distance(fields.begin(), res)] = true;
            }
        }
    }
    return validated + check_fields(fields, contains);
}

namespace pred
{
    auto byr(std::string_view s)
        -> bool
    {
        int year;
        auto [p, ec] = std::from_chars(s.begin(), s.end(), year);
        if (p != s.end())
            return false;
        if (year < 1920 || year > 2002)
            return false;
        return true;
    }
    auto iyr(std::string_view s)
        -> bool
    {
        int year;
        auto [p, ec] = std::from_chars(s.begin(), s.end(), year);
        if (p != s.end())
            return false;
        if (year < 2010 || year > 2020)
            return false;
        return true;
    }
    auto eyr(std::string_view s)
        -> bool
    {
        int year;
        auto [p, ec] = std::from_chars(s.begin(), s.end(), year);
        if (p != s.end())
            return false;
        if (year < 2020 || year > 2030)
            return false;
        return true;
    }
    auto hgt(std::string_view s)
        -> bool
    {
        int height;
        auto [p, ec] = std::from_chars(s.begin(), s.end(), height);

        if (p == s.begin())
            return false;
        std::string_view rest(p, s.end());
        if (rest == "cm")
        {
            if (height < 150 || height > 193)
                return false;
            else
                return true;
        }
        else if (rest == "in")
        {
            if (height < 59 || height > 76)
                return false;
            else
                return true;
        }
        else
        {
            return false;
        }
    }
    auto hcl(std::string_view s)
        -> bool
    {
        std::array<char, 16> pool = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
        if (s.size() != 7)
            return false;
        if (s[0] != '#')
            return false;
        for (auto &c : s.substr(1))
        {
            if (rg::find(pool, c) == pool.end())
                return false;
        }
        return true;
    }
    auto ecl(std::string_view s)
        -> bool
    {
        std::array<std::string_view, 7> pool = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        if (s.size() != 3)
            return false;
        return rg::find(pool, s) != pool.end();
    }
    auto pid(std::string_view s)
        -> bool
    {
        if (s.size() != 9)
            return false;
        return rg::all_of(s, [](auto &c) {
            return c >= '0' && c <= '9';
        });
    }
} // namespace pred

auto solve_2(const std::string &path)
    -> std::size_t
{
    using namespace pred;
    const std::array<std::string, 7> fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    std::array<bool, 7> contains = {false, false, false, false, false, false, false};
    static const std::array<std::function<bool(std::string_view)>, 7> preds = {byr, iyr, eyr, hgt, hcl, ecl, pid};
    std::string line;
    line.reserve(2000);
    bool current_valid = true;
    std::ifstream file(path);
    std::size_t validated = 0;

    while (std::getline(file, line))
    {
        if (line.size() == 0)
        {
            validated += check_fields(fields, contains);
            rg::fill(contains, false);
            current_valid = true;
        }
        else if (current_valid)
        {
            static boost::char_separator<char> sep(" ");
            for (const auto &str : boost::tokenizer(line, sep))
            {
                std::string_view field(str.data(), str.data() + 3);
                auto res = rg::find(fields, field);
                if (res != fields.end())
                {
                    std::string_view data(str.data() + 4, str.data() + str.length());
                    auto idx = std::distance(fields.begin(), res);
                    if (preds[idx](data))
                        contains[idx] = true;
                    else
                        current_valid = false;
                }
            }
        }
    }
    return validated + check_fields(fields, contains);
}

auto split(const std::string &str, const std::regex &reg)
    -> std::vector<std::string>
{
    return std::vector<std::string>(
        std::sregex_token_iterator(str.begin(), str.end(), reg, -1),
        std::sregex_token_iterator());
}

auto main(int, char **)
    -> int
{
    auto vec = split("lol xd jos", std::regex("\\s+"));
    /*for (auto &thing : vec)
        std::cout << thing << std::endl;
        */

    std::cout << std::endl;

    std::cout << solve_2("./input") << std::endl;

    //std::cout << benchmark(solve_2, "./input") << "[µs]." << std::endl;

    std::cout << open("file") << std::endl;

    return 0;
}