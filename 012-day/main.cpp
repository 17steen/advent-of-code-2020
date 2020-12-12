#include <algorithm>
#include <array>
#include <boost/tokenizer.hpp>
#include <cassert>
#include <charconv>
#include <cmath>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
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

constexpr auto pi_180 = (M_PIf64 / 180);

constexpr inline auto to_radian(double deg)
	-> double
{
	return deg * pi_180;
}

using point = struct point
{
	double x;
	double y;
	const point &operator+=(point other) noexcept
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	const point &rotate_origin(double degrees) noexcept
	{
		double rad = to_radian(degrees);
		double s = std::sin(rad);
		double c = std::cos(rad);

		point tmp{x, y};
		tmp.x = x * c - y * s;
		tmp.y = x * s + y * c;
		*this = tmp;
		return *this;
	}
	point operator*(int amount) const noexcept
	{
		point tmp = *this;
		tmp.x *= amount;
		tmp.y *= amount;
		return tmp;
	}
};

auto operator<<(std::ostream &out, const point &pt)
	-> std::ostream &
{
	out << pt.x << ", " << pt.y;
	return out;
}

auto solve(const std::string &path)
	-> std::pair<std::size_t, std::size_t>
{
	std::size_t part_1 = 0;
	std::size_t part_2 = 0;

	double current_rotation_p1 = 90; // E
	double x_p1 = 0;
	double y_p1 = 0;

	double rad_tmp;

	point ship = {0, 0};
	point wp = {10, 1}; // E 10 N 1

	auto file = iter_file(path);
	for (std::string_view str : file)
	{

		if (str.length() == 0)
			break;
		char ch = str.at(0);

		int num;
		auto [p, ec] = std::from_chars(&str.at(1), &*str.end(), num);
		//assert that no errors occured
		assert(ec == std::errc{});

		switch (ch)
		{
		case 'F':
			rad_tmp = to_radian(current_rotation_p1);
			x_p1 += num * std::sin(rad_tmp);
			y_p1 += num * std::cos(rad_tmp);

			ship += wp * num;

			break;
		case 'R':
			current_rotation_p1 += num;

			wp.rotate_origin(-num);
			break;
		case 'L':
			current_rotation_p1 -= num;

			wp.rotate_origin(num);
			break;
		case 'N':
			y_p1 += num;

			wp.y += num;
			break;
		case 'E':
			x_p1 += num;

			wp.x += num;
			break;
		case 'S':
			y_p1 -= num;

			wp.y -= num;
			break;
		case 'W':
			x_p1 -= num;

			wp.x -= num;
			break;
		default:
			std::exit(ch);
		}
	}

	part_1 = std::abs(std::round(x_p1)) + std::abs(std::round(y_p1));
	part_2 = std::abs(std::round(ship.x)) + std::abs(std::round(ship.y));

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
