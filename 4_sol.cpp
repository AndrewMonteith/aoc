#include "util.hpp"

#include <algorithm>
#include <charconv>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>

std::optional<int> ParseNumber(std::string_view s)
{
    int num;

    auto result = std::from_chars(s.data(), s.data() + s.size(), num);
    if (result.ec == std::errc::invalid_argument)
    {
        return std::nullopt;
    }
    else
    {
        return num;
    }
};

bool strNumberBetween(std::string_view s, int low, int high)
{
    auto numberOpt = ParseNumber(s);
    return numberOpt.has_value() && (low <= *numberOpt && *numberOpt <= high);
}

using namespace std::placeholders;

static std::unordered_map<std::string_view, std::function<bool(std::string_view)>>
    PassportFieldValidators = {{"byr", std::bind(strNumberBetween, _1, 1920, 2002)},
                               {"iyr", std::bind(strNumberBetween, _1, 2010, 2020)},
                               {"eyr", std::bind(strNumberBetween, _1, 2020, 2030)},
                               {"pid",
                                [](std::string_view sv) {
                                    return sv.size() == 9 && strNumberBetween(sv, 1, 999'999'999);
                                }},
                               {"hgt",
                                [](std::string_view sv) {
                                    if (sv.size() <= 2)
                                    {
                                        return false;
                                    }

                                    if (sv.ends_with("cm"))
                                    {
                                        sv.remove_suffix(2);
                                        return strNumberBetween(sv, 150, 193);
                                    }
                                    else if (sv.ends_with("in"))
                                    {
                                        sv.remove_suffix(2);
                                        return strNumberBetween(sv, 59, 76);
                                    }
                                    else
                                    {
                                        return false;
                                    }
                                }},
                               {"hcl",
                                [](std::string_view sv) {
                                    return std::regex_match(sv.data(), sv.data() + sv.size(),
                                                            std::regex("#[0-9a-f]{6}"));
                                }},
                               {"ecl", [](std::string_view sv) {
                                    static std::unordered_set<std::string_view> s{
                                        "amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
                                    return s.contains(sv);
                                }}};

class PassportValidator
{
    std::string buffer = "";
    std::function<bool(std::string_view)> passportPredicate;

  public:
    PassportValidator(std::function<bool(std::string_view)> passportPredicate)
        : passportPredicate(passportPredicate){};

    bool operator()(std::string line)
    {
        bool valid = false;

        if (line.empty())
        {
            valid = passportPredicate(buffer);
            buffer.clear();
        }
        else
        {
            buffer.push_back(' ');
            buffer += line;
        }

        return valid;
    }
};

static std::vector REQUIRED_FIELDS = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

bool isValidPassportDay1(std::string_view passport)
{
    return std::ranges::all_of(REQUIRED_FIELDS, std::identity{},
                               [&passport](const std::string &entry) {
                                   return passport.find(entry) != std::string::npos;
                               });
};

bool isValidPassportDay2(std::string_view passport)
{
    passport.remove_prefix(passport.find_first_not_of(' '));
    // std::cout << "Passport " << passport << '\n';

    const auto isValidField = [](std::string_view s) {
        // std::cout << "Field " << s << ' ' << s.size() << '\n';
        const auto colon = s.find(':');
        const auto key = s.substr(0, colon);
        if (key == "cid")
        {
            return true;
        }
        // std::cout << "Field " << s << ' ' << PassportFieldValidators.at(key)(s.substr(colon + 1)) << '\n';

        return PassportFieldValidators.at(key)(s.substr(colon + 1));
    };

    return std::ranges::all_of(util::SplitString(passport, ' '), isValidField);
}

int main()
{
    std::vector<std::string> lines = util::parse("4_input", std::identity{});
    lines.push_back(""); // Force process the last one.

    std::cout << std::ranges::count(lines, true, PassportValidator(isValidPassportDay1)) << '\n';
    std::cout << std::ranges::count(lines, true, PassportValidator(isValidPassportDay2)) << '\n';
}