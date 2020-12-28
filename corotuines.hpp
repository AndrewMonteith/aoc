#include <coroutine>


// #ifndef INCLUDED_UTIL_H
// #define INCLUDED_UTIL_H

// #include <type_traits>
// #include <concepts>
// #include <filesystem>
// #include <iterator>
// #include <fstream>
// #include <string>
// #include <vector>


// namespace util {

// template <typename Object>
// using Parser = Object (*)(const std::string&);

// template <typename Object, template <typename> typename Container=std::vector>
// Container<Object> parse(const std::string& fileName, Parser<Object> parser) {
//     auto container = Container<Object>();
//     auto inserter = std::back_inserter(container);

//     auto f = std::ifstream(std::filesystem::current_path() / fileName);
//     std::string line;
//     while (std::getline(f, line)) {
//         *inserter++ = parser(line);
//     }

//     return container;
// }

// };

// #endif
