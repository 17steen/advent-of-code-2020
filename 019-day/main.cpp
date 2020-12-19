#include <algorithm>
#include <array>
#include <bits/c++config.h>
#include <bits/stdint-uintn.h>
#include <boost/tokenizer.hpp>
#include <cassert>
#include <charconv>
#include <chrono>
#include <cmath>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iomanip>
#include <ios>
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
#include <sys/types.h>
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

struct rule
{
	uint32_t id;
	std::vector<std::vector<uint32_t>> pipe;
	//if vec empty, char to match.
	char match;
};

auto get_rules(std::ifstream &file)
	-> std::unordered_map<uint32_t, struct rule>
{
	static auto line = std::string{};
	std::unordered_map<uint32_t, struct rule> rules{};

	for (;;)
	{
		std::getline(file, line);

		if (line.length() == 0)
		{
			break;
		}

		uint32_t rule_id;

		const char *after_colon;

		{
			auto [p, ec] = std::from_chars(&*line.begin(), &*line.end(), rule_id);
			assert(ec == std::errc{});

			after_colon = p + 1;
		}

		//skip n:
		auto rest = std::string_view(after_colon, &*line.end());

		struct rule rule = {.id = rule_id, .pipe{}, .match = '\0'};

		std::vector<uint32_t> ids{};

		for (auto it = rest.begin(); it < rest.end(); ++it)
		{
			char ch = *it;

			if (ch == ' ')
			{
				continue;
			}

			else if (ch >= '0' and ch <= '9')
			{
				uint32_t num;
				auto [p, ec] = std::from_chars(it, rest.end(), num);
				assert(ec == std::errc{}); // assert that no errors

				it = std::string_view::iterator(p - 1); // loop

				ids.push_back(num);

				continue;
			}

			else if (ch == '|')
			{
				rule.pipe.emplace_back(std::move(ids));
				ids = std::vector<uint32_t>{};
			}

			else if (ch == '"')
			{
				//subject to change
				rule.match = *++it;
				//std::cout << " ( " << rule.match << " ) " << std::endl;
				++it;
			}
			// doesn't reach
			else
			{
				std::cerr << "err char : " << ch << std::endl;
			}
		}

		if (ids.size() != 0) //
		{
			rule.pipe.emplace_back(std::move(ids));
		}

		rules.emplace(rule_id, std::move(rule));
	}

	return rules;
}

auto validate_string(const std::unordered_map<uint32_t, struct rule> &rules, const struct rule &to_follow, std::string_view str)
	-> std::vector<std::string_view>
{
	if (str.empty())
	{
		return {};
	}
	if (!to_follow.pipe.size()) // no sub rule, thus use char
	{
		if (to_follow.match == str.at(0))
		{
			return {str.substr(1)};
		}
		else
		{
			return {};
		}
	}
	else
	{
		std::vector<std::string_view> outcomes{};

		for (const auto &sublist : to_follow.pipe)
		{
			auto currents = std::vector<std::string_view>{str};

			for (const auto &rule_id : sublist)
			{
				const auto &rule = rules.at(rule_id);

				auto results = std::vector<std::string_view>{};

				for (auto &curr : currents)
				{
					auto vec = validate_string(rules, rule, curr);
					std::copy(vec.begin(), vec.end(), std::back_inserter(results));
				}

				currents = results;
			}
			std::copy(currents.begin(), currents.end(), std::back_inserter(outcomes));
		}
		return outcomes;
	}

	std::cerr << "should not have reached here …………… terminating …………" << std::endl;
	std::terminate();

	return {};
}

void print_rules(const std::unordered_map<uint32_t, struct rule> &rules)
{
	for (auto &pair : rules)
	{
		auto &rule = pair.second;
		std::cout << rule.id << ": ";
		if (rule.pipe.size())
		{
			for (auto &sublist : rule.pipe)
			{
				for (uint32_t id : sublist)
				{
					std::cout << id << ", ";
				}
				std::cout << " | ";
			}
		}
		else
		{
			std::cout << "\"" << rule.match << "\"";
		}

		std::cout << '\n';
	}
	std::cout << std::endl;
}

auto solve(const std::string &path)
	-> std::pair<std::size_t, std::size_t>
{
	std::size_t part_1 = 0;
	std::size_t part_2 = 0;

	static auto line = std::string{};

	auto file = std::ifstream(path, std::ios::in);

	auto rules = get_rules(file);

	auto rules_p2 = rules;

	rules_p2.at(8) = rule{
		.id = 8,
		.pipe{
			{42},
			{42, 8},
		},
		.match = 0,
	};

	rules_p2.at(11) = rule{
		.id = 11,
		.pipe{
			{42, 31},
			{42, 11, 31},
		},
		.match = 0,
	};

	/*/ print rules
	print_rules(rules_p2);
	//*/

	while (std::getline(file, line))
	{
		{
			auto vec = validate_string(rules, rules.at(0), line);

			if (std::any_of(vec.begin(), vec.end(), [](const std::string_view sv) { return sv.empty(); }))
			{
				part_1 += 1;
			}
		}

		{
			auto vec = validate_string(rules_p2, rules_p2.at(0), line);
			bool b;
			if ((b = rg::any_of(vec, [](const std::string_view sv) { return sv.empty(); })))
			{
				part_2 += 1;
			}
			//std::cout << (b ? "✔ " : "🤬 ") << " : " << line << std::endl;
		}
	}
	std::cout << std::endl;
	//*/

	return {part_1, part_2};
}

auto main(int, char **)
	-> int
{

	//	std::cout << rule().first << std::endl;

	//std::cout << std::boolalpha << validate_string({}, rule{.id = 0, .pipe{}, .match = 'z'}, "a").first << std::endl;

	//*/
	{
		time_start(solving);
		auto [part_1, part_2] = solve("./input");

		std::cout << part_1 << std::endl;
		std::cout << part_2 << std::endl;

		time_end(solving, "execution time : ");
	}
	//*/
	return 0;
}