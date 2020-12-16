#include <algorithm>
#include <array>
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

#define loop while (true)

namespace rg = std::ranges;
namespace vw = std::views;

#define time_start(x) auto x = std::chrono::high_resolution_clock::now();

#define time_end(x, str) std::cout << (str) << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (x)).count() << "[µs].\n";

struct range
{
	std::intmax_t start;
	std::intmax_t end;
};

constexpr bool in_range(struct range range, std::intmax_t num)
{
	return num >= range.start and num <= range.end;
}

struct field
{
	bool departure;
	std::string name;
	std::size_t index;
	struct range range_left;
	struct range range_right;
	constexpr bool contains(std::size_t num) const noexcept
	{
		return in_range(range_left, num) or in_range(range_right, num);
	}

	constexpr auto operator<=>(const struct field &other) const noexcept
		-> std::weak_ordering
	{
		return index <=> other.index;
	}
	constexpr auto operator==(const struct field &other) const noexcept
		-> bool
	{
		return index == other.index;
	}
};

struct hasher_field
{
	constexpr auto operator()(const struct field &field) const noexcept
		-> std::size_t
	{
		return field.index;
	}
};

auto get_ticket(std::ifstream &file)
	-> std::string
{
	std::string string;
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(file, string);
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return string;
}

auto iota_set(std::size_t n)
	-> std::unordered_set<std::size_t>
{
	auto set = std::unordered_set<std::size_t>(n);
	std::size_t j = 0;
	std::generate_n(std::inserter(set, set.begin()), n, [&j] { return j++; });
	return set;
}

auto get_fields(std::ifstream &file)
	-> std::unordered_map<struct field, std::unordered_set<std::size_t>, struct hasher_field>
{
	std::unordered_map<struct field, std::unordered_set<std::size_t>, struct hasher_field> fields{};

	//fields
	std::size_t index = 0;
	std::string string;
	while (true)
	{

		std::getline(file, string);
		if (string.length() == 0)
			break;

		bool departure = string.starts_with("departure");

		std::size_t colon_pos = string.find(':');

		std::string rule_name(string.begin(), string.begin() + colon_pos);

		std::string_view ranges_str(&string.at(colon_pos + 2), &*string.end());
		struct range range_left;

		std::size_t dash_pos = ranges_str.find('-');

		{
			auto [p, ec] = std::from_chars(&*ranges_str.begin(), &(ranges_str.at(dash_pos)), range_left.start);
			assert(ec == std::errc{});
		}

		std::size_t or_pos = ranges_str.find("or", dash_pos);
		{
			auto [p, ec] = std::from_chars(&ranges_str.at(dash_pos + 1), &ranges_str.at(or_pos), range_left.end);
			assert(ec == std::errc{});
		}

		struct range range_right;
		dash_pos = ranges_str.rfind('-');
		{
			auto [p, ec] = std::from_chars(&ranges_str.at(or_pos + 3), &ranges_str.at(dash_pos), range_right.start);
			assert(ec == std::errc{});
		}
		{
			auto [p, ec] = std::from_chars(&ranges_str.at(dash_pos + 1), &*ranges_str.end(), range_right.end);
			assert(ec == std::errc{});
		}

		struct field field = {.departure = departure,
							  .name = rule_name,
							  .index = index,
							  .range_left = range_left,
							  .range_right = range_right};

		fields.emplace(field, std::unordered_set<std::size_t>{});

		++index;
	}

	for (auto &[key, value] : fields)
	{
		value = iota_set(fields.size());
	}

	return fields;
}

template <typename T>
void pprint_range(char ch1, T range, char ch2)
{
	std::cout << ch1;
	for (auto it = range.cbegin(); it != range.cend(); ++it)
	{
		std::cout << *it;

		std::cout << ", ";
	}

	std::cout << ch2;
}

auto solve(const std::string &path)
	-> std::pair<std::size_t, std::size_t>
{
	std::size_t part_1 = 0;
	std::size_t part_2 = 1;

	auto file = std::ifstream(path, std::ios::in);
	auto string = std::string{};

	//get the fields
	auto fields = get_fields(file);

	//my ticket
	auto my_ticket = get_ticket(file);

	//nearby tickets:
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	auto numbers = std::vector<std::size_t>(fields.size(), std::numeric_limits<std::size_t>::max());

	auto ticket_n = 0UL;
	while (std::getline(file, string))
	{
		bool invalid = false;

		auto tokens = boost::tokenizer(string, boost::char_separator<char>(","));
		for (std::size_t i = 0; const auto &number : tokens)
		{
			std::size_t val;
			auto [p, ec] = std::from_chars(&*number.begin(), &*number.end(), val);
			assert(ec == std::errc{});

			invalid = rg::none_of(fields, [&val](const auto &field) {
				return field.first.contains(val);
			});

			numbers.at(i) = val;

			if (invalid) // discared
			{

				part_1 += val;
				break;
			}
			++i;
		}

		if (not invalid)
		{
			for (std::size_t i = 0; i < numbers.size(); ++i)
			{
				for (auto &[field, matches] : fields)
				{
					auto num = numbers.at(i);
					if (not field.contains(num))
					{
						matches.erase(i);
					}
				}
			}
		}
		++ticket_n;
	}

	while (true)
	{
		for (auto &[field, set] : fields)
		{
			if (set.size() == 1)
			{
				auto certified_item = *set.begin();
				for (auto &[f, s] : fields)
				{
					if (std::addressof(set) != std::addressof(s))
						s.erase(certified_item);
				}
			}
		}
		if (rg::none_of(fields, [](const auto &map) { return map.second.size() > 1; }))
		{
			break;
		}
	}

	//parsing ticket
	for (auto i = 0UL; auto num : boost::tokenizer(my_ticket, boost::char_separator<char>(",")))
	{
		std::size_t val;
		std::from_chars(&*num.begin(), &*num.end(), val);
		numbers.at(i) = val;
		++i;
	}

	for (auto &[field, value] : fields)
	{
		if (field.departure)
		{
			part_2 *= numbers.at(*value.begin());
		}
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