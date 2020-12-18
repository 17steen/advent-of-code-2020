#include <algorithm>
#include <array>
#include <boost/tokenizer.hpp>
#include <cassert>
#include <chrono>
#include <charconv>
#include <cmath>
#include <compare>
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
#include <unistd.h>
#include <string_view>
#include <system_error>
#include <unordered_map>
#include <vector>
#include <thread>

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

auto parse_p2(const std::string &expr)
	-> intmax_t
{
	int str_to_sh[2];
	int res_from_sh[2];
	int ec = pipe(str_to_sh);
	assert(ec == 0);
	ec = pipe(res_from_sh);
	assert(ec == 0);

	int pid = fork();
	assert(pid != -1);
	if (pid == 0) /* child */
	{
		dup2(str_to_sh[0], STDIN_FILENO);	 // reads from str instead of stdin
		dup2(res_from_sh[1], STDOUT_FILENO); //writes to res_from_sh instead of stdout
		execl("./line_echo", "./line_echo", nullptr);

		std::cerr << "did not execute" << std::endl;

		exit(1);
	}
	else
	{
		close(str_to_sh[0]);
		close(res_from_sh[1]);
		auto amount_written = write(str_to_sh[1], expr.c_str(), expr.size());
		std::cerr << amount_written << std::endl;

		char buffer[255] = {};

		auto amount_read = read(res_from_sh[0], static_cast<void *>(buffer), 10);
		std::cerr << "read a few bytes : " << amount_read << std::endl;
		std::cerr << buffer << std::endl;

		close(str_to_sh[1]);
		close(res_from_sh[0]);
	}
	return 0;
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

			printf("%ld ? %ld = ", val, tmp);
			val = operand(val, tmp);
			printf("% ld\n", val);

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

auto solve(const std::string &path)
	-> std::pair<std::size_t, std::size_t>
{
	std::size_t part_1 = 0;
	std::size_t part_2 = 0;

	auto file = std::ifstream(path, std::ios::in);
	auto line = std::string{};

	while (std::getline(file, line))
	{
		std::cout << "uinarset" << std::endl;
		part_1 += parse(line).second;
	}

	return {part_1, part_2};
}

auto main(int, char **)
	-> int
{

	parse_p2("hahhahhaah");

	/*
	{
		time_start(solving);
		auto [part_1, part_2] = solve("./input");

		std::cout << part_1 << std::endl;
		std::cout << part_2 << std::endl;

		time_end(solving, "execution time : ");
	}
*/
	return 0;
}