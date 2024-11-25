#include <iostream>
#include <string>
#include <array>
/**
 * Example 1: Template Specialization.
 */
template <typename T>
struct is_floating_point
{
    constexpr static bool value = false;
};
template <>
struct is_floating_point<float>
{
    constexpr static bool value = true;
};
template <>
struct is_floating_point<double>
{
    constexpr static bool value = true;
};
template <>
struct is_floating_point<long double>
{
   constexpr static bool value = true;
};

/**
 * Example 2: Static class members can be fully specialized. However, each specialization has its own copy of any static members.
 */
template <typename T>
struct foo
{
    static T val;
};

// General specialization.
template <typename T> T foo<T>::val = 0;

// Explicit specialization.
template <> float foo<float>::val = 22;

/**
 * Example 3: Partial specialization multiple params.
 */
template <typename U, typename W>
void func(U a, W b)
{
    std::cout << "Primary Template called" << std::endl;
}
template <>
void func(int a, double b)
{
    std::cout << "int-double called" << std::endl;
}
template <typename U>
void func(U a, double b)
{
    std::cout << "Template double called" << std::endl;
}


/**
 * Example 4: Working example.
 * print [, , , ] when array provided.
 * print [text] when string provided.
 */
template <typename T, size_t S>
void pretty_print(std::ostream& os, const std::array<T,S>& arr)
{
    std::cout << "[";
    if(S > 0)
    {
        for(size_t i=0; i<S;++i)
        {
            os << arr[i] << ",";
        }
    }
    std::cout << "]" << std::endl;
}

template <size_t S>
std::ostream& pretty_print(std::ostream& os, const std::array<char,S>& arr)
{
    std::cout << "[";
    if(S > 0)
    {
        for(size_t i=0; i<S;++i)
        {
            os << arr[i];
        }
    }
    std::cout << "]" << std::endl;
    return os;
}

/**
 * Example 5: A function that takes list with delimeter (can be chars of different types)
 */
template <typename T>
constexpr T DELIMETER = '\n';

template <>
constexpr wchar_t DELIMETER<wchar_t> = L'\n';

template <typename T>
std::basic_ostream<T>& show_parts(std::basic_ostream<T>& os, const std::basic_string_view<T>& ss)
{
    using size_type = typename std::basic_string_view<T>::size_type;
    size_type start = 0;
    size_type end;
    do
    {
        end = ss.find(DELIMETER<T>, start);
        os << ss.substr(start, end-start);
        start = end + 1;
    } while (end != std::string::npos);
    os << std::endl;
    return os;
}

/**
 * Example 6: Using variable template to simplify code.
 *      previously we used is_floating_point template, but that needs ::value
 *      to access the variable.
 *      We can use template variable to simplify this.
 */
template <typename T>
struct is_integral_type
{
    constexpr static bool value = false;
};

template <>
struct is_integral_type<int>
{
    constexpr static bool value = true;   
};

/**
 * WHY INLINE?
 * In C++, non-inline constexpr variables have internal linkage. However, if you include the template definition
 * in multiple translation units (via headers), this can lead to multiple definitions of the same variable and linker errors.
 * Adding inline ensures that the variable has external linkage and avoids multiple definition errors.
 * This is particularly important for template-based variables like is_integral_v.
 * 
 * inline in this context works similarly to inline for functions. It allows the variable to be defined in a header file,
 * included in multiple translation units, without causing multiple definitions at link time.
 * 
 * Templates are instantiated when used. If is_integral_v is used in multiple translation units,
 * each instantiation might lead to separate definitions without inline.
 * Adding inline tells the compiler that all instances of is_integral_v in different translation units are the same and can be merged.
 */

/**
 * When to Use inline for constexpr Variables?

    Header Files:
        If the constexpr variable template is defined in a header file and included in multiple .cpp files, use inline to avoid linker errors.
        Example:

        template <typename T>
        inline constexpr bool is_integral_v = is_integral_type<T>::value;

    Global constexpr Variables:
        If the variable is intended to be globally accessible across multiple files, use inline.

    When Sharing Templates:
        For template variable definitions like is_integral_v, which are common and meant for generic programming, always prefer inline.
 */
template <typename T>
inline constexpr bool is_integral_v = is_integral_type<T>::value; 

int main()
{
    // Example 1
    std::cout << is_floating_point<int>::value << std::endl;
    std::cout << is_floating_point<float>::value << std::endl;

    // Example 2
    foo<int> f_int;
    foo<float> f_float;
    foo<double> f_double;
    std::cout << f_int.val << std::endl;
    std::cout << f_float.val << std::endl;
    std::cout << f_double.val << std::endl;

    // Example 3 
    func(std::string{"hello"}, std::string{"world"});
    func(1.0f, 1.0);

    // Example 4
    std::array<double, 5> dba{1.0, 2.0, 3.0, 4.0, 5.0};
    std::array<char, 5> dbs{'T','E','M','P','L'};
    pretty_print(std::cout, dba);
    pretty_print(std::cout, dbs);

    // Example 5
    show_parts<char>(std::cout, "one\ntwo\nthree");

    // Example 6
    std::cout << is_integral_v<int> << std::endl;
    std::cout << is_integral_v<float> << std::endl;

    return 0;
}