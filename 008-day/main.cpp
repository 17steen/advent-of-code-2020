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

using model = struct model
{
    int64_t accumulator;
    std::size_t instruction_ptr;
};

using instruction = struct instruction
{
    model (*f)(model, int);
    int op;
    bool was_used;
};

auto solve(const std::string &path)
    -> std::pair<std::size_t, std::size_t>
{
    std::size_t part_1 = 0, part_2 = 0;

    auto file = iter_file(path, std::ios::in);

    constexpr auto nop = [](model s, int) -> model { return {s.accumulator, s.instruction_ptr + 1}; };
    constexpr auto acc = [](model s, int op) -> model { return {s.accumulator + op, s.instruction_ptr + 1}; };
    constexpr auto jmp = [](model s, int op) -> model { return {s.accumulator, s.instruction_ptr + op}; };

    std::vector<instruction> instructions;
    for (std::string_view line : file)
    {
        int op;
        char c = line.at(0);
        char sign = line.at(4);
        std::from_chars(&line.at(5), &*line.end(), op);
        op *= sign == '+' ? 1 : -1;

        switch (c)
        {
        case 'n':
            instructions.emplace_back(instruction{nop, op, false});
            break;
        case 'a':
            instructions.emplace_back(instruction{acc, op, false});
            break;
        case 'j':
            instructions.emplace_back(instruction{jmp, op, false});
            break;
        }
    }

    auto iter = instructions.begin();
    instruction instr_back = *instructions.begin();
    bool go = true;
    do
    {
        model state = {0, 0};
        instruction prev;
        do
        {
            auto &instr = instructions.at(state.instruction_ptr);
            auto &[f, op, used] = instr;

            if (used)
            {
                std::for_each(instructions.begin(), instructions.end(), [](auto &in) { in.was_used = false; });
                if (iter == instructions.begin())
                    part_1 = state.accumulator;
                break;
            }

            state = f(state, op);
            if (state.instruction_ptr == instructions.size())
            {
                go = false;
                part_2 = state.accumulator;
                goto mdr;
            }
            used = true;
            prev = instr;

        } while (true);

        *iter = instr_back;
        auto modified = std::find_if(iter + 1, instructions.end(), [jmp, nop](instruction &instr) { return instr.f == jmp || instr.f == nop; });

        if (modified == instructions.end())
            [[unlikely]] // bad
            {
                std::exit(1);
            }
        instr_back = *modified;
        modified->f = modified->f == jmp ? nop : jmp;
        iter = modified;
    } while (go);
mdr:
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