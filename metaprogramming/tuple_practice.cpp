#include <iostream>
#include <functional>
#include <string>
#include <type_traits>
#include <sstream>
#include <vector>

/**
 * Task: write a function that can convert input of variable types and variable
 * sizes into a string.
 * eg: auto str = make_string("my fav ", " number is ", 21);
 */

 template<typename Tuple, typename Func, size_t Index=0>
 void tuple_for_each(const Tuple& tuple, const Func& func)
 {
    constexpr size_t n = std::tuple_size_v<Tuple>;
    if constexpr(Index < n)
    {
        func(std::get<Index>(tuple));
        tuple_for_each<Tuple, Func, Index+1>(tuple, func);
    }
 }

 template <typename... Args>
 std::string make_string(Args... args)
 {
    auto tuple = std::tie(args...);
    std::stringstream ss;
    tuple_for_each(tuple, [&ss](const auto& v){ ss << v; });
    return ss.str();
 }

int main(int argc, char** argv)
{
    std::string output = make_string("Hello ","I am ","Siddhesh ", " and ", " I am ", 29, " yrs old.");
    std::cout << output << std::endl;
    return 0;
}