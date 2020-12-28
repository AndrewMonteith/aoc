#ifndef INCLUDED_UTIL_H
#define INCLUDED_UTIL_H

#include <concepts>
#include <cppcoro/generator.hpp>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

template <>
inline constexpr bool std::ranges::enable_borrowed_range<cppcoro::generator<std::string_view>> =
    true;

namespace util
{

cppcoro::generator<std::string_view> SplitString(std::string_view text, char delimeter)
{
    auto *left = text.data();
    auto *right = left;

    for (auto ii = 0; ii < text.size(); ++ii, ++right)
    {
        if (*right != delimeter)
        {
            continue;
        }

        const auto windowSize = std::distance(left, right);
        if (windowSize > 0)
        {
            co_yield std::string_view(left, windowSize);
        }

        left = ++right;
    }

    const auto returnedChars = std::distance(text.data(), left);
    if (returnedChars < text.size())
    {
        co_yield std::string_view(left, text.size() - returnedChars);
    }
}

template <template <typename> typename Container = std::vector>
auto parse(const std::string &fileName, std::invocable<std::string> auto &&parser)
{
    using Object = std::decay_t<std::invoke_result_t<decltype(parser), std::string>>;

    auto container = Container<Object>();
    auto inserter = std::back_inserter(container);

    auto f = std::ifstream(std::filesystem::current_path() / fileName);
    std::string line;
    while (std::getline(f, line))
    {
        *inserter++ = parser(line);
    }

    return container;
}

}; // namespace util

#endif
