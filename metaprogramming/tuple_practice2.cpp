// #include <iostream>
// #include <functional>
// #include <string>
// #include <type_traits>
// #include <sstream>
// #include <vector>

// template <typename T, typename = void>
// struct is_iterable : std::false_type {};

// template <typename T>
// struct is_iterable <T, std::void_t< decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>())) >> : std::true_type {};

// // Explicitly mark const char* and char* as non-iterable
// template <>
// struct is_iterable<const char*> : std::false_type {};

// template <>
// struct is_iterable<char*> : std::false_type {};

// template <typename T>
// std::enable_if<!is_iterable<T>::value, std::string> get_string(const T& val)
// {
//     std::stringstream ss;
//     ss << val;
//     return ss.str();
// }

// template <typename T>
// std::enable_if<is_iterable<T>::value, std::string> get_string(const T& val)
// {
//     std::stringstream ss;
//     for(auto& v: val)
//     {
//         ss << v;
//     }
//     return ss.str();
// }

// // Special overload for const char* to avoid ambiguity
// std::string get_string(const char* val)
// {
//     return std::string(val);
// }

// // Special overload for char* to avoid ambiguity
// std::string get_string(char* val)
// {
//     return std::string(val);
// }

// template <typename Tuple, typename Func, size_t Index=0>
// void for_each_tuple(const Tuple& tuple, const Func& func)
// {
//     constexpr size_t n = std::tuple_size_v<Tuple>;
//     if constexpr(Index < n)
//     {
//         func(std::get<Index>(tuple));
//         for_each_tuple<Tuple, Func,  Index+1>(tuple, func);
//     }
// }

// template <typename ...Args>
// std::string get_string_args(Args ...args)
// {
//     auto tuple = std::tie(args...);
//     std::stringstream ss;
//     for_each_tuple(tuple, [&ss](auto&& v){ ss << get_string(v);});
//     return ss.str();
// }

// int main(int argc, char** argv)
// {
//     std::string str = get_string_args("Hi ","I ","am ", "Siddhesh ","My fav numbers are ", std::vector<int>{3,21});
//     return 0;
// }

#include <iostream>
#include <functional>
#include <string>
#include <type_traits>
#include <sstream>
#include <vector>

// Correct definition of is_iterable trait
template <typename T, typename = void>
struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), 
                                 decltype(std::end(std::declval<T>()))>> : std::true_type {};

// Template for non-iterable types
template <typename T>
std::enable_if_t<!is_iterable<T>::value, std::string> get_string(const T& val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

// Template for iterable types
template <typename T>
std::enable_if_t<is_iterable<T>::value, std::string> get_string(const T& val)
{
    std::stringstream ss;
    for(const auto& v: val)
    {
        ss << v;
    }
    return ss.str();
}

template <typename Tuple, typename Func, size_t Index=0>
void for_each_tuple(const Tuple& tuple, const Func& func)
{
    constexpr size_t n = std::tuple_size_v<Tuple>;
    if constexpr(Index < n)
    {
        func(std::get<Index>(tuple));
        for_each_tuple<Tuple, Func, Index+1>(tuple, func);
    }
}

template <typename ...Args>
std::string get_string_args(Args ...args)
{
    auto tuple = std::tie(args...);
    std::stringstream ss;
    for_each_tuple(tuple, [&ss](auto&& v){ ss << get_string(v);});
    return ss.str();
}

int main(int argc, char** argv)
{
    std::string str = get_string_args("Hi ","I ","am ", "Siddhesh ","My fav numbers are ", std::vector<int>{3,21},std::array<float,2>{96.0,33.33f});
    std::cout << str << std::endl;
    return 0;
}