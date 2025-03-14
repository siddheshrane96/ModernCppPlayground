#include <iostream>
#include <tuple>
#include <string>
#include <algorithm>
#include <functional>
#include <type_traits>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

template <size_t Index, typename Tuple, typename Func>
void tuple_at(const Tuple& tuple, Func func)
{
    const auto& v = std::get<Index>(tuple);
    std::invoke(func, v);
}

template <typename Tuple, typename Func, size_t Index=0>
void for_each_tuple(const Tuple& tuple, Func func)
{
    constexpr size_t n = std::tuple_size_v<Tuple>;
    if constexpr(Index < n)
    {
        tuple_at<Index>(tuple, func);
        for_each_tuple<Tuple, Func, Index+1>(tuple, func);
    }
}

template <typename Tuple, typename Func, size_t Index = 0>
bool any_of_tuple(const Tuple& tuple, Func func)
{
    constexpr size_t n = std::tuple_size_v<Tuple>;
    if constexpr (Index < n) {
        const auto& v = std::get<Index>(tuple);
        if constexpr (std::is_invocable_v<Func, decltype(v)>) {
            if (std::invoke(func, v))
            {
                return true;
            }
        }
        return any_of_tuple<Tuple, Func, Index + 1>(tuple, func);
    }
    return false;
}

int main(int agrc, char** argv)
{
    std::tuple tup={1,std::string{"str"}, 1.3f};
    auto tup2 = std::make_tuple("hello", "world", "and", "siddhesh",1.2f, 3.3f);
    auto f = [](auto& v){ std::cout << v << std::endl;};
    
    for_each_tuple(tup2, f);

    auto f2 = [](const auto& v) {
        if constexpr (std::is_same_v<decltype(v), const char*> || std::is_same_v<decltype(v), std::string>) {
            return v == std::string("siddhesh");
        }
        return false;
    };
    
    any_of_tuple(tup2, f2);

    return 0;
}