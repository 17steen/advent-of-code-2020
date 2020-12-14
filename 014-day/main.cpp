#include <algorithm>
#include <array>
#include <bits/c++config.h>
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <boost/tokenizer.hpp>
#include <cassert>
#include <charconv>
#include <cmath>
#include <compare>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <math.h>
#include <memory>
#include <numeric>
#include <ostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include <cstdio>
#include <unordered_set>

#include "../includes/helpers.hpp"
namespace rg = std::ranges;
namespace vw = std::views;

#define time_start(x) auto x = std::chrono::high_resolution_clock::now();

#define time_end(x, str) std::cout << (str) << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (x)).count() << "[µs].\n";

void get_masks(const std::string &line, std::size_t &mask_0, std::size_t &mask_1)
{
	mask_1 = std::numeric_limits<std::size_t>::max();
	mask_0 = std::numeric_limits<std::size_t>::min();
	std::size_t num;

	auto tmp = line.substr(7);
	std::for_each(tmp.begin(), tmp.end(), [](char &c) { if(c == 'X') c = '0'; });
	{
		auto [p, ec] = std::from_chars(&*tmp.begin(), &*tmp.end(), num, 2);
		assert(ec == std::errc{});
	}
	mask_0 |= num;

	tmp = line.substr(7);
	std::for_each(tmp.begin(), tmp.end(), [](char &c) { if(c == 'X') c = '1'; });
	{
		auto [p, ec] = std::from_chars(&*tmp.begin(), &*tmp.end(), num, 2);
		assert(ec == std::errc{});
	}
	mask_1 &= num;
}

auto get_x_es(const std::string line)
	-> std::vector<std::size_t>
{
	auto tmp = line.substr(7);
	rg::reverse(tmp);
	std::vector<std::size_t> vec;
	for (size_t i = 0; auto ch : tmp)
	{
		if (ch == 'X')
		{
			vec.emplace_back(i);
		}
		++i;
	}
	return vec;
}

auto solve(const std::string &path)
	-> std::pair<std::size_t, std::size_t>
{
	std::size_t part_1 = 0;
	std::size_t part_2 = 0;

	auto file = std::ifstream(path, std::ios::in);

	auto memory_p1 = std::unordered_map<std::size_t, std::size_t>{};
	auto memory_p2 = std::unordered_map<std::size_t, std::size_t>{};
	auto line = std::string{};

	std::size_t mask_0;
	std::size_t mask_1;

	std::vector<std::size_t> x_pos{};

	while (std::getline(file, line))
	{
		if (line.starts_with("mask"))
		{
			//part1
			get_masks(line, mask_0, mask_1);

			// part 2
			x_pos = get_x_es(line);
		}
		else
		{
			std::size_t memaddr;
			auto [p, ec] = std::from_chars(&*line.begin() + 4, &*line.end(), memaddr, 10);
			assert(ec == std::errc{});

			std::size_t num, value;
			auto [p2, ec2] = std::from_chars(p + 4, &*line.end(), num);
			assert(ec2 == std::errc{});

			//part 1
			value = num;
			value &= mask_1;
			value |= mask_0;

			memory_p1.insert_or_assign(memaddr, value);

			//part 2
			memaddr |= mask_0;

			//set all X's to zero
			for (auto x : x_pos)
			{
				memaddr &= ~((1UL) << x);
			}

			std::size_t possibilities = 2 << x_pos.size();
			for (auto i = 0UL; i < possibilities; ++i)
			{
				std::size_t copy = memaddr;
				for (auto j = 0UL; auto x : x_pos)
				{
					// i is gonna represent the current possibility, map each
					// base 2 digit to an x;

					copy |= ((i >> j) & 1) << x;
					++j;
				}

				memory_p2.insert_or_assign(copy, num);
			}
		}
	}

	for (auto [addr, val] : memory_p1)
	{
		part_1 += val;
	}

	for (auto [addr, val] : memory_p2)
	{
		part_2 += val;
	}
	return {part_1, part_2};
}

auto main(int, char **)
	-> int
{

	{
		time_start(solving);
		auto [part_1, part_2] = solve("./input");

		std::cout << part_1 << std::endl;
		std::cout << part_2 << std::endl;

		time_end(solving, "execution time : ");
	}

	return 0;
}
