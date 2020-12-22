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
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <exception>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
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

using deck_type = std::deque<uint32_t>;

auto hash_deck(const deck_type &dq)
	-> uint64_t
{
	uint64_t hash = 0;
	for (uint64_t v : dq)
	{
		hash |= (uint64_t)1 << v;
	}
	return hash;
}

auto get_deck(std::ifstream &file)
	-> deck_type
{
	static auto buffer = std::string{};

	auto deck = deck_type{};

	std::getline(file, buffer);
	while (std::getline(file, buffer))
	{
		if (buffer.length() == 0)
		{
			return deck;
		}
		uint32_t num;

		auto [p, ec] = std::from_chars(&*buffer.begin(), &*buffer.end(), num);
		assert(ec == std::errc{});

		deck.emplace_back(num);
	}

	return deck;
}

void print_iterable(rg::range auto &iterable)
{
	for (auto &v : iterable)
	{
		std::cout << v << '\n';
	}
}

auto get_score(rg::bidirectional_range auto &bidi)
{
	auto acc = 0UL;
	for (auto count = 1; auto &val : bidi | vw::reverse)
	{
		acc += val * count;
		++count;
	}
	return acc;
}

auto get_part1(deck_type p1_deck, deck_type p2_deck)
{
	auto part_1 = std::size_t{};

	for (auto turn = 0UL;; ++turn)
	{
		if (p2_deck.empty())
		{
			part_1 = get_score(p1_deck);
			break;
		}
		else if (p1_deck.empty())
		{
			part_1 = get_score(p2_deck);
			break;
		}

		//pop no return … 😥
		auto card_1 = p1_deck.front();
		p1_deck.pop_front();

		auto card_2 = p2_deck.front();
		p2_deck.pop_front();

		if (card_1 > card_2)
		{
			p1_deck.push_back(card_1);
			p1_deck.push_back(card_2);
		}
		else if (card_2 > card_1)
		{
			p2_deck.push_back(card_2);
			p2_deck.push_back(card_1);
		}
		else
		{
			std::cerr << "cards are equal !!!" << std::endl;
			std::terminate();
		}
	}

	return part_1;
}

enum winner
{
	P1,
	P2
};

auto recurse(deck_type &p1_deck, deck_type &p2_deck)
	-> winner
{
	auto history = std::vector<std::pair<uint64_t, uint64_t>>{};
	for (auto turn = 0UL;; ++turn)
	{
		if (p2_deck.empty())
		{
			return P1;
		}
		else if (p1_deck.empty())
		{
			return P2;
		}
		auto h1 = hash_deck(p1_deck);
		auto h2 = hash_deck(p2_deck);
		//Before either player deals a card, if there was a previous round in this game that had exactly the same cards in the same order in the same players' decks, the game instantly ends in a win for player 1. Previous rounds from other games are not considered. (This prevents infinite games of Recursive Combat, which everyone agrees is a bad idea.)
		for (auto &[p1, p2] : history)
		{
			if (h1 == p1 and h2 == p2)
			{
				return P1;
			}
		}
		//save it to history
		history.push_back({h1, h2});

		//Otherwise, this round's cards must be in a new configuration; the players begin the round by each drawing the top card of their deck as normal.

		//pop no return … 😥
		auto card_1 = p1_deck.front();
		p1_deck.pop_front();

		auto card_2 = p2_deck.front();
		p2_deck.pop_front();

		winner w;

		//If both players have at least as many cards remaining in their deck as the value of the card they just drew, the winner of the round is
		if (card_1 <= p1_deck.size() and card_2 <= p2_deck.size())
		{
			//recurse
			auto cp1 = deck_type(p1_deck.begin(), p1_deck.begin() + card_1);
			//print_iterable(cp1);
			auto cp2 = deck_type(p2_deck.begin(), p2_deck.begin() + card_2);
			w = recurse(cp1, cp2);
		}

		//Otherwise, at least one player must not have enough cards left in their deck to recurse; the winner of the round is the player with the higher-value card
		else if (card_1 > card_2)
		{
			w = P1;
		}
		else if (card_2 > card_1)
		{
			w = P2;
		}
		else
		{
			std::cerr << "cards are equal !!!" << std::endl;
			std::terminate();
		}

		if (w == P1)
		{
			p1_deck.push_back(card_1);
			p1_deck.push_back(card_2);
		}
		else
		{
			p2_deck.push_back(card_2);
			p2_deck.push_back(card_1);
		}
	}
}

auto get_part2(deck_type p1_deck, deck_type p2_deck)
	-> std::size_t
{
	winner w = recurse(p1_deck, p2_deck);
	if (w == P1)
	{
		return get_score(p1_deck);
	}
	else //w == P2
	{
		return get_score(p2_deck);
	}
}

auto solve(const std::string &path)
	-> std::pair<std::size_t, std::size_t>
{
	std::size_t part_1 = 0;
	std::size_t part_2 = 0;

	static auto line = std::string{};

	auto file = std::ifstream(path, std::ios::in);

	const auto p1_deck = get_deck(file);
	const auto p2_deck = get_deck(file);

	part_1 = get_part1(p1_deck, p2_deck);
	part_2 = get_part2(p1_deck, p2_deck);

	return {part_1, part_2};
}

auto main(int, char **)
	-> int
{
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
