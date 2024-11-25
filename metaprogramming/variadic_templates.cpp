#include <iostream>
#include <string>
#include <vector>

/**
 * NOTES:
 * 1. typename... Args is called Template Parameter pack.
 * 2. Args... args is called function parameter pack.
 * 3. args... inside function body is called parameter pack expansion.
 */

/// Generic cout function
template <typename... Args>
void LOG_ME(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

template <typename T>
T min(const T& a, const T&b)
{
    return (a < b) ? a:b;
}

template <typename T, typename... Args>
T min(T a, Args... args)
{
    return min(a, min(args...));
}

/***
 * 
From the call min(1, 5, 3, -4, 9), the compiler is instantiating a set of overloaded functions with 5, 4, 3, and 2 arguments.
Conceptually, it is the same as having the following set of overloaded functions:

int min(int a, int b)
{
   return a < b ? a : b;
}
int min(int a, int b, int c)
{
   return min(a, min(b, c));
}
int min(int a, int b, int c, int d)
{
   return min(a, min(b, min(c, d)));
}
int min(int a, int b, int c, int d, int e)
{
   return min(a, min(b, min(c, min(d, e))));
}

As a result, min(1, 5, 3, -4, 9) expands to min(1, min(5, min(3, min(-4, 9)))). 
 */

int main(int argc, char** argv)
{
    auto f = min(1.2f, 2.2f, 3.3f, 4.4f);
    LOG_ME("MIN F ", f);
    return 0;
}