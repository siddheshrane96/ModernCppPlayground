#include <iostream>

/**
 * The comma operator (`,`) evaluates its left-hand operand and discards the result,
 * then evaluates its right-hand operand. We can use it to apply an operation in reverse order.
 */
template <typename... Args>
void print_left_to_right(Args... args)
{
    (std::cout << ... << args) << std::endl;
    // alternate
    // ((std::cout << args), ...) << std::endl;
}

// RECURSIVE APPROACH. NAIVE
template <typename T>
void print_right_to_left(T arg)
{
    std::cout << arg;
}

template <typename T, typename... Args>
void print_right_to_left(T head, Args... args)
{
    print_right_to_left(args...);
    std::cout << head;
}

int main(int argc, char** argv)
{
    print_left_to_right(1,2,3,4,5);
    print_right_to_left(1,2,3,4,5);
    return 0;
}