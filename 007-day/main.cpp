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

using bag = struct bag
{
    std::size_t colour_hash;
    int contains_our_friend;
    std::vector<std::pair<std::size_t, std::size_t>> bags;
};

auto is_my_bag_in_there(std::vector<bag> &head, decltype(bag::bags) &idc, std::size_t hash)
    -> bool
{
    for (auto [idx, amount] : idc)
    {

        auto &curr = head.at(idx);
        switch (curr.contains_our_friend)
        {
        case 0:
            return true;
        case 1:
            break;
        case -1:
            if (curr.colour_hash == hash)
            {
                curr.contains_our_friend = 0;
                return true;
            }
            if (is_my_bag_in_there(head, curr.bags, hash))
            {
                curr.contains_our_friend = 0;
                return true;
            }
            else
            {
                curr.contains_our_friend = 1;
            }
        }
    }
    return false;
}

auto count_bags(std::vector<bag> &head, decltype(bag::bags) &idc)
    -> std::size_t
{
    std::size_t count = 0;
    for (auto [idx, amount] : idc)
    {
        auto curr = head.at(idx);
        count += amount;
        count += amount * count_bags(head, curr.bags);
    }
    return count;
}

auto solve(const std::string &path)
    -> std::pair<std::size_t, std::size_t>
{
    static std::hash<std::string> hasher;

    auto all_the_bags = std::vector<bag>{};
    all_the_bags.reserve(100);

    std::size_t size = 1000;
    char *buffer = (char *)malloc(size);
    std::size_t pos = 0;

    std::FILE *fp = fopen(path.data(), "r");
    int ret = 0;

    do
    {
        char name[40], colour[20], amount[10];

        ret = fscanf(fp, "%s %s bags contain", name, colour);

#pragma GCC diagnostic ignored "-Wunused-result"
        getline(&buffer, &size, fp);
        pos = 0;

        if (ret != 2)
        {
            break;
        }

        //std::cerr << " >> the buffer :  " << buffer << std::endl;

        sscanf(buffer, "%s", amount);

        std::strncat(name, colour, 39);
        std::size_t hash = hasher(name);
        int amount_n = 0;

        std::size_t curr_bag;

        if (auto it = rg::find_if(all_the_bags, [&hash](bag &other) { return other.colour_hash == hash; }); it != all_the_bags.end())
        {
            curr_bag = std::distance(all_the_bags.begin(), it);
            //std::cerr << "bag existed !" << std::endl;
        }
        else
        {
            all_the_bags.emplace_back(bag{.colour_hash = hash, /*.colour = name,*/ .contains_our_friend = -1, .bags{}});
            curr_bag = all_the_bags.size() - 1;
        }

        if (amount[0] >= '0' && amount[0] <= '9')
        {
            auto amount_strlen = std::strlen(amount);

            bool go_on;
            do
            {
                sscanf(buffer + pos, "%s %s %s", amount, name, colour);
                std::from_chars(amount, amount + amount_strlen, amount_n);
                std::strcat(name, colour);

                std::size_t other_hash = hasher(name);
                if (auto it = rg::find_if(all_the_bags, [&other_hash](bag &other) { return other.colour_hash == other_hash; }); it != all_the_bags.end())
                {
                    all_the_bags.at(curr_bag).bags.emplace_back(std::distance(all_the_bags.begin(), it), amount_n);
                }
                else
                {
                    all_the_bags.emplace_back(bag{.colour_hash = other_hash, /*.colour = name,*/ .contains_our_friend = -1, .bags{}});
                    std::size_t bag_ptr = all_the_bags.size() - 1;
                    all_the_bags.at(curr_bag).bags.emplace_back(bag_ptr, amount_n);
                }
                char *tmp = buffer + pos;
                {
                loop:
                    if (*tmp == ',')
                    {
                        go_on = true;
                    }
                    else if (*tmp == '.')
                    {
                        go_on = false;
                    }
                    else
                    {
                        ++tmp;
                        ++pos;
                        goto loop;
                    }
                }
                ++pos;
            } while (go_on);
        }
    } while (true);

    free(buffer);

    std::size_t part_1 = 0;
    std::size_t part_2 = 0;

    auto shiny_gold = hasher("shinygold");

    for (auto &sacosj : all_the_bags)
    {
        bool inc_val = 0;
        if (sacosj.contains_our_friend == 0)
        {
            inc_val = 1;
        }
        else if (sacosj.contains_our_friend == 1)
        {
            inc_val = 0;
        }
        else
        {
            inc_val = is_my_bag_in_there(all_the_bags, sacosj.bags, shiny_gold);
        }
        part_1 += inc_val;

        if (sacosj.colour_hash == shiny_gold)
        {
            part_2 = count_bags(all_the_bags, sacosj.bags);
        }
    }
    part_1 -= 1; //shiny gold shouldn’t be counted
    return {part_1, part_2};
}

auto main(int, char **)
    -> int
{

    time_start(solving);
    auto [part1, part2] = solve("input");
    time_end(solving, "time to solve: ");

    //std::cout << "both part benchmark : " << benchmark(solve, "input") << "[µs]." << std::endl;

    std::cout << "part 1 : " << part1 << '\n'
              << "part 2 : " << part2 << std::endl;

    return 0;
}