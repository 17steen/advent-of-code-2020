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
#include <vector>

#include <cstdio>
#include <unordered_set>

#include "../includes/helpers.hpp"
namespace rg = std::ranges;
namespace vw = std::views;

#define time_start(x) auto x = std::chrono::high_resolution_clock::now();

#define time_end(x, str) std::cout << (str) << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (x)).count() << "[µs].\n";

struct bus
{
	intmax_t id;
	intmax_t wait;

	std::weak_ordering operator<=>(const bus &other) const
	{
		return wait <=> other.wait;
	}
};

auto solve(const std::string &path)
	-> std::pair<std::size_t, std::size_t>
{
	std::size_t part_1 = 0;
	std::size_t part_2 = 0;

	auto file = std::ifstream(path, std::ios::in);

	std::string line;

	long int time_stamp;
	std::getline(file, line);

	std::from_chars(&*line.begin(), &*line.end(), time_stamp);

	std::getline(file, line);

	struct bus earliest
	{
		std::numeric_limits<intmax_t>::max(), std::numeric_limits<intmax_t>::max()
	};

	std::vector<std::pair<std::size_t, std::size_t>> busses;

	int minute_offset = -1;
	for (const auto &bus_str : boost::tokenizer(line))
	{
		++minute_offset;
		if (bus_str.at(0) == 'x')
			continue;

		// PART 1
		struct bus bus;
		std::from_chars(&*bus_str.begin(), &*bus_str.end(), bus.id);

		bus.wait = bus.id - (time_stamp % bus.id);

		earliest = std::min(bus, earliest);
		// PART 1

		//PART 2
		busses.emplace_back(bus.id, minute_offset);
	}

	part_1 = earliest.id * earliest.wait;

	intmax_t t = 0;
	intmax_t inc = busses.at(0).first; // id
	auto to_satisfy = busses.begin() + 1;

	while (true)
	{
		auto &[id, offset] = *to_satisfy;

		if ((id - (t % id)) % id == offset % id)
		{
			inc *= id;
			++to_satisfy;

			if (to_satisfy == busses.end())
				break;
		}
		t += inc;
	}

	part_2 = t;

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
