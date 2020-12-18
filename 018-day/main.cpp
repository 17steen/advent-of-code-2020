#include <algorithm>
#include <array>
#include <boost/tokenizer.hpp>
#include <cassert>
#include <charconv>
#include <chrono>
#include <cmath>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include <cstdio>
#include <unordered_set>

#include "../includes/helpers.hpp"

#define loop while (true)

namespace rg = std::ranges;
namespace vw = std::views;

using namespace std::literals::chrono_literals;

#define time_start(x) auto x = std::chrono::high_resolution_clock::now();

#define time_end(x, str) std::cout << (str) << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (x)).count() << "[µs].\n";
constexpr auto
plus(intmax_t a, intmax_t b) noexcept
	-> intmax_t
{
	return a + b;
}

constexpr auto mul(intmax_t a, intmax_t b) noexcept
	-> intmax_t
{
	return a * b;
}

auto parse(std::string_view expression)
	-> std::pair<decltype(expression)::iterator, intmax_t>
{
	auto operand = plus;

	intmax_t val = 0;

	auto it = expression.begin();
	while (it < expression.end())
	{
		auto ch = *it;
		if (ch == ')')
		{
			return {it, val};
		}
		//proper parsing to come
		if (ch >= '0' and ch <= '9')
		{
			intmax_t tmp;
			auto [p, ec] = std::from_chars(&*it, &*expression.end(), tmp);
			if (ec != std::errc{})
			{
				std::cerr << "num parsing failed………" << std::endl;
				std::exit(1);
			}

			// from_chars returns where it stopped, very handy
			it = std::string_view::iterator(p);

			//printf("%ld ? %ld = ", val, tmp);
			val = operand(val, tmp);
			//printf("%ld\n", val);

			continue;
		}
		else if (ch == '+')
		{
			operand = plus;
		}
		else if (ch == '*')
		{
			operand = mul;
		}
		else if (ch == ' ')
		{
		}
		else if (ch == '(')
		{
			auto [at, res] = parse({it + 1, expression.end()});
			it = at;
			val = operand(val, res);
		}
		else
		{
		}
		++it;
	}
	return {it, val};
}

auto seek_left(const std::string::iterator start, const std::string::iterator current)
	-> std::string
{
	int bracket_count = 0;

	for (auto it = current - 1; it >= start;)
	{
		char ch = *it;
		if (ch == ')')
		{
			++bracket_count;
		}
		else if (ch == '(')
		{
			--bracket_count;
			if (bracket_count == 0)
			{
				// do the thing
				return std::string(start, it) + '(' + std::string(it, current);
			}
		}
		else if (ch >= '0' and ch <= '9')
		{

			do
			{
				--it;
				ch = *it;

			} while (ch >= '0' and ch <= '9');

			if (bracket_count == 0 and it >= start)
			{
				//std::cerr << "get here uwu owo awa abababab" << std::endl;
				return std::string(start, it) + '(' + std::string(it, current);
			}

			continue;
		}
		--it;
	}

	return '(' + std::string(start, current);
}

auto seek_right(const std::string::iterator end, const std::string::iterator current)
	-> std::string
{
	int bracket_count = 0;

	for (auto it = current + 1; it != end;)
	{
		char ch = *it;
		if (ch == '(')
		{
			++bracket_count;
		}
		else if (ch == ')')
		{
			--bracket_count;
			if (bracket_count == 0)
			{
				return std::string(current + 1, it) + ')' + std::string(it, end);
			}
		}
		else if (ch >= '0' and ch <= '9')
		{
			do
			{
				++it;
				ch = *it;

			} while (ch >= '0' and ch <= '9');

			if (bracket_count == 0 and it != end)
			{
				return std::string(current + 1, it) + ')' + std::string(it, end);
			}

			continue;
		}
		++it;
	}

	return std::string(current + 1, end) + ')';
}

auto add_parenthesese(std::string str)
	-> std::string
{
	size_t skip = 0;

	for (;;)
	{
		auto p_idx = std::find(str.begin() + skip, str.end(), '+');

		if (p_idx == str.end())
		{
			return str;
		}

		auto left_part = seek_left(str.begin(), p_idx);

		skip = left_part.size() + 2;

		str = left_part + '+' + seek_right(str.end(), p_idx);
	}
}

auto solve(const std::string &path)
	-> std::pair<std::size_t, std::size_t>
{
	std::size_t part_1 = 0;
	std::size_t part_2 = 0;

	auto file = std::ifstream(path, std::ios::in);
	auto line = std::string{};

	while (std::getline(file, line))
	{
		part_1 += parse(line).second;
		auto res = add_parenthesese(line);
		part_2 += parse(res).second;
	}

	return {part_1, part_2};
}

auto main(int, char **)
	-> int
{
	/*
	std::string str{"(2 * (2)) + 2"};

	std::cout << seek_left(str.begin(), str.begin() + str.find('+')) << '+';
	std::cout << seek_right(str.end(), str.begin() + str.find('+'));
	*/

	{
		time_start(solving);
		auto [part_1, part_2] = solve("./input");

		std::cout << part_1 << std::endl;
		std::cout << part_2 << std::endl;

		time_end(solving, "execution time : ");
	}

	return 0;
}