#include <iostream>
#include <vector>

template <typename T, typename=void>
struct is_iterable: std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
    decltype(std::end(std::declval<T>()))>>: std::true_type {};

int main(int argc, char** argv)
{
    std::cout << is_iterable<int>::value << std::endl;
    std::cout << is_iterable<std::vector<int>>::value << std::endl;
    return 0;
}