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
auto split(const std::string &str, const std::regex &reg)
    -> std::vector<std::string>
{
    return std::vector<std::string>(
        std::sregex_token_iterator(str.begin(), str.end(), reg, -1),
        std::sregex_token_iterator());
}

auto solve(const std::string &path)
    -> std::pair<uint16_t, uint16_t>
{
    auto file = iter_file(path, std::ios::in);

    //  PART 2  //
    std::array<uint16_t, 1024> arr;
    auto curr = arr.begin();
    // PART 2   //

    uint16_t max_id = 0;

    for (std::string_view v : file)
    {
        if (v.length() != 10)
            [[unlikely]] continue;
        char row[7];
        char col[3];
        rg::uninitialized_copy(v.substr(0, 7), row);
        rg::uninitialized_copy(v.substr(7), col);
        for (char &c : row)
        {
            if (c == 'F')
                c = '0';
            else
                c = '1';
        }
        for (char &c : col)
        {
            if (c == 'L')
                c = '0';
            else
                c = '1';
        }
        uint16_t row_n;
        std::from_chars(row, row + sizeof(row), row_n, 2);
        uint16_t col_n;
        std::from_chars(col, col + sizeof(col), col_n, 2);

        uint16_t seat_id = (row_n << sizeof(col)) | col_n;

        max_id = std::max(seat_id, max_id);

        //  PART 2  //
        *curr = seat_id;
        ++curr;
        //          //
    }

    // PART 2 //
    uint16_t my_seat;
    std::sort(arr.begin(), curr);
    for (uint16_t prev = arr.front(); auto id : arr | vw::drop(1))
    {
        if (prev != id - 1)
        {
            my_seat = id - 1;
            break;
        }
        prev = id;
    }
    // PART 2 //

    return {max_id, my_seat};
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