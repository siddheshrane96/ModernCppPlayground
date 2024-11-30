#include <iostream>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

/**
 * Example 1: Compile time factorial function using struct.
 */
template <unsigned int N>
struct factorial
{
    static constexpr unsigned int value = factorial<N-1>::value * N;
};

template <>
struct factorial<0>
{
    static constexpr unsigned int value = 1;
};

/**
 * Example 2: Factorial using template variable.
 */
template <unsigned int N>
inline constexpr unsigned int fact_var = fact_var<N-1>*N;

template <>
inline constexpr unsigned int fact_var<0> = 1;

/**
 * Example 3: Using function templates
 */
template <unsigned int N>
constexpr unsigned int fact_func()
{
    return N * fact_func<N-1>();
}

template <>
constexpr unsigned int fact_func<1>()
{
    return 1;
}

template <>
constexpr unsigned int fact_func<0>()
{
    return 1;
}

int main(int argc, char** argv)
{
    LOG("Fact 4: ", factorial<4>::value);
    LOG("Fact 11: ", factorial<11>::value);

    LOG("FACT VAR 3: ", fact_var<3>);

    LOG("FACT FUNC 5: ", fact_func<5>());
    return 0;
}