#include "util.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric_ranges.hpp>
#include <ranges>
#include <vector>

#include <cppcoro/generator.hpp>

namespace ranges = std::ranges;

template <> inline constexpr bool ranges::enable_borrowed_range<cppcoro::generator<bool>> = true;

using Slope = std::vector<std::string>;

cppcoro::generator<bool> DescendSlope(const Slope &slope, std::pair<int, int> dir)
{
    auto x = 0, y = 0;
    auto [dx, dy] = dir;

    while (y < std::size(slope))
    {
        co_yield slope[y][x] == '#';

        y += dy;
        x = (x + dx) % std::size(slope[0]);
    };
}

long CountTrees(const Slope &slope, std::pair<int, int> dir)
{
    return ranges::count(DescendSlope(slope, dir), true);
}

long Part2(const Slope &slope)
{
    using namespace std::placeholders;

    auto traversals = std::vector<std::pair<int, int>>{{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};

    return tcb::accumulate(traversals, 1l, std::multiplies<long>{},
                           std::bind(CountTrees, slope, _1));
}

int main()
{
    Slope slope = util::parse("3_input", std::identity{});

    std::cout << CountTrees(slope, {3, 1}) << '\n'; // Part 1
    std::cout << Part2(slope) << '\n';
}
