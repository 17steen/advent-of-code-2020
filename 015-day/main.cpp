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
#include <memory>
#include <ostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <unordered_map>
#include <vector>

#include <cstdio>
#include <unordered_set>

#include "../includes/helpers.hpp"
namespace rg = std::ranges;
namespace vw = std::views;

#define time_start(x) auto x = std::chrono::high_resolution_clock::now();

#define time_end(x, str) std::cout << (str) << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (x)).count() << "[µs].\n";

auto solve(const std::string &path)
	-> std::pair<std::size_t, std::size_t>
{
	std::size_t part_1 = 0;
	std::size_t part_2 = 0;

	auto file = std::ifstream(path, std::ios::in);
	std::string tmp{};
	std::getline(file, tmp);

	constexpr std::size_t max_iter_p1 = 2020UL;
	constexpr std::size_t max_iter_p2 = 30'000'000UL;

	std::unordered_map<std::size_t, std::size_t> numbers;
	numbers.reserve(4'000'000UL);

	std::size_t current_iter = 0;
	std::size_t spoken;

	decltype(numbers)::iterator last_inserted;

	for (const auto &str : boost::tokenizer(tmp))
	{
		std::size_t num;

		auto [p, ec] = std::from_chars(&*str.begin(), &*str.end(), num);
		assert(ec == std::errc{});

		last_inserted = numbers.insert_or_assign(num, current_iter).first;
		spoken = num;
		//printf("Turn %lu: %lu.\n", current_iter + 1, num);
		++current_iter;
	}
	numbers.erase(last_inserted);

	for (; current_iter < max_iter_p2; ++current_iter)
	{
		const auto res = numbers.find(spoken);

		if (max_iter_p1 == current_iter)
		{
			part_1 = spoken;
		}

		if (res == numbers.end()) // not found;
		{
			numbers.insert_or_assign(spoken, current_iter - 1);
			spoken = 0;
		}
		else
		{
			auto key = res->second;

			numbers.insert_or_assign(spoken, current_iter - 1);
			spoken = current_iter - 1 - key; //second is iteration
		}
	}
	part_2 = spoken;

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
