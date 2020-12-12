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
#include <thread>

namespace rg = std::ranges;
namespace vw = std::views;

#define time_start(x) auto x = std::chrono::high_resolution_clock::now();

#define time_end(x, str) std::cout << (str) << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (x)).count() << "[µs].\n";

enum seat
{
    floor,
    empty,
    occupied
};

auto parse(const std::string &path)
    -> std::vector<std::vector<seat>>
{
    std::ifstream file(path, std::ios::in);
    std::string str;

    std::vector<std::vector<seat>> outer;
    do
    {

        std::vector<seat> inner;
        std::getline(file, str);
        if (str.length() == 0)
        {
            break;
        }
        for (char ch : str)
        {
            switch (ch)
            {
            case 'L':
                inner.emplace_back(seat::empty);
                break;
            case '#':
                inner.emplace_back(seat::occupied);
                break;
            case '.':
                inner.emplace_back(seat::floor);
                break;

            default:
                std::terminate();
                break;
            }
        }
        outer.emplace_back(std::move(inner));

    } while (!file.eof());
    return outer;
}

auto is_adjacent(const std::vector<std::vector<seat>> &seats, long i, long j, seat type)
    -> bool
{
    for (long h = i - 1; h <= i + 1; ++h)
    {
        if (h < 0 || h >= seats.size())
        {
            continue;
        }
        for (long w = j - 1; w <= j + 1; ++w)
        {
            if (w < 0 || w >= seats[h].size() || (i == h && j == w))
            {
                continue;
            }
            //std::cout << seats.at(h).at(w) << " | " << type << std::endl;
            if (seats.at(h).at(w) == type)
            {
                return true;
            }
        }
    }
    return false;
}

auto sees_seat(const std::vector<std::vector<seat>> &seats, long i, long j, seat type)
    -> bool
{
    static std::pair<int, int> moves[] = {{0, 1}, {1, 1}, {1, 0}, {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {1, -1}};
    long h, w;
    for (const auto &move : moves)
    {

        h = i + move.first;
        w = j + move.second;
        while (h < seats.size() && w < seats[h].size() && h >= 0 && w >= 0)
        {
            auto s = seats[h][w];
            if (s == type)
                return true;
            else if (s != floor)
                break;
            h += move.first;
            w += move.second;
        }
    }
    return false;
}

auto seen_count(const std::vector<std::vector<seat>> &seats, long i, long j, seat type)
    -> long
{
    static std::pair<int, int> moves[] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
    long count = 0;
    long h, w;
    for (const auto &move : moves)
    {
        h = i + move.first;
        w = j + move.second;
        while (h < seats.size() && w < seats[h].size() && h >= 0 && w >= 0)
        {
            auto s = seats[h][w];
            count += s == type;
            if (s != floor)
                break;
            h += move.first;
            w += move.second;
        }
    }
    return count;
}

auto adjacent_count(const std::vector<std::vector<seat>> &seats, long i, long j, seat type)
    -> long
{
    long count = 0;
    for (long h = i - 1; h <= i + 1; ++h)
    {
        if (h < 0 || h >= seats.size())
        {
            continue;
        }
        for (long w = j - 1; w <= j + 1; ++w)
        {
            if (w < 0 || w >= seats[h].size() || i == h && j == w)
            {
                continue;
            }
            if (seats.at(h).at(w) == type)
            {
                ++count;
            }
        }
    }
    return count;
}
auto print(seat s)
{
    static char c[] = ".L#";
    return c[s];
}
auto solve(const std::string &path)
    -> std::pair<std::size_t, std::size_t>
{
    std::size_t part_1 = 0;
    std::size_t part_2 = 0;
    auto array = parse(path);

    bool go_on;
    do
    {
        auto prev = array;
        go_on = false;

        for (long i = 0; i < array.size(); ++i)
        {
            for (long j = 0; j < array[0].size(); ++j)
            {
                switch (prev[i][j])
                {
                case empty:
                    if (!sees_seat(prev, i, j, seat::occupied))
                    {
                        go_on = true;
                        array[i][j] = seat::occupied;
                    }
                    break;
                case occupied:
                    if (seen_count(prev, i, j, seat::occupied) >= 5)
                    {
                        go_on = true;
                        array[i][j] = seat::empty;
                    }
                    break;
                default:;
                }
            }
        }
    } while (go_on);
    /*
    std::ofstream out("./john");
    for (auto &a : array)
    {
        for (auto &b : a)
        {
            part_1 += b == seat::occupied;
            out << print(b);
        }
        out << '\n';
    }
    */
    return {part_1, part_2};
}

auto main(int, char **)
    -> int
{
    time_start(solving);
    auto [part_1, part_2] = solve("./input");

    std::cout << part_1 << std::endl;
    std::cout << part_2 << std::endl;

    time_end(solving, "execution time :");

    return 0;
}
