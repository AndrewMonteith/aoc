#include "util.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

struct PasswordPolicy
{
    int i1;
    int i2;
    char letter;
    std::string password;
};

PasswordPolicy parse(const std::string &line)
{
    std::stringstream stream(line);

    int i1, i2;
    stream >> i1;
    stream.ignore(1) >> i2;

    char letter;
    stream >> letter;

    std::string password;
    stream.ignore(1) >> password;

    return PasswordPolicy{i1, i2, letter, std::move(password)};
}

int main()
{
    namespace ranges = std::ranges;

    std::vector<PasswordPolicy> p = util::parse("2_input", parse);

    const auto isValidDay1 = [](const PasswordPolicy &policy) {
        const auto letterFreq = ranges::count(policy.password, policy.letter);
        return policy.i1 <= letterFreq && letterFreq <= policy.i2;
    };

    const auto isValidDay2 = [](const PasswordPolicy &policy) {
        const auto letter1 = policy.password[policy.i1 - 1];
        const auto letter2 = policy.password[policy.i2 - 1];

        return (letter1 == policy.letter && letter2 != policy.letter) ||
               (letter2 == policy.letter && letter1 != policy.letter);
    };

    std::cout << "Day 1: " << ranges::count_if(p, isValidDay1) << '\n';
    std::cout << "Day 2: " << ranges::count_if(p, isValidDay2) << '\n';
}
