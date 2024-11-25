#include <iostream>
#include <string>
#include <algorithm>

/**
 * NOTE:
 *  The most commonly used character types are:

    char: Regular characters.
    wchar_t: Wide characters for Unicode text (platform-dependent).
    char8_t (C++20): UTF-8 encoded characters.
    char16_t (C++11): UTF-16 encoded characters.
    char32_t (C++11): UTF-32 encoded characters.
 */

/**
 * NOTE:
 *  Two ways to aliasa std::cout << ..... << std::endl;
 * 
 * 1. Variadic template
 * template <typename... Args>
 * void log_me(Args&&... args)
 * {
 *  (std::cout << ... << args) << std::endl;
 * }
 * 
 */

template <typename... Args>
void log_me(Args&&... args)
{
    (std::cout << ... << args) << std::endl;
}

template <typename T>
void log_list(T&& arr)
{
    for(auto a: arr)
        std::cout << a << " ";
    std::cout << std::endl;
}

///// 1. Write a function that takes a string and prints the ASCII value of each character.
template <typename T>
std::basic_ostream<T>& print_ascii_values(std::basic_ostream<T>& os, const std::basic_string_view<T>& str)
{
    for(auto c: str)
    {
        os << static_cast<int>(c) << " ";
    }
    os << std::endl;
    return os;
}

///// 2. Reverse a string in place.
void reverse_string(std::string& str)
{
    auto begin = str.begin();
    auto end = str.end();
    --end;
    while(begin<end)
    {
        // std::swap(*begin, *end);
        std::iter_swap(begin, end);
        ++begin;
        --end;
    }

    // Alternatively we can use
    // std::reverse(str.begin(), str.end());
}

///// 3. Write a function to trim leading and trailing spaces from a string.
std::string trim_string(const std::string& str)
{
    // ____strsdasa____
    auto start = str.find_first_not_of(" ");
    auto end = str.find_last_not_of(" ");
    return (start == std::string::npos) ? "" : str.substr(start, end-start+1);
}

//// 4. Write a function to split a string by DELIMETER.
std::vector<std::string> split_string(const std::string& str, char delim)
{
    std::vector<std::string> ans;
    auto start = 0;
    auto end = start;
    while(end != std::string::npos)
    {
        end = str.find_first_of(delim, start);
        ans.push_back(str.substr(start, end-start));
        start = end+1;
    }
    return ans;
}

int main(int argc, char** argv)
{
    /// 1. Example 1.
    print_ascii_values<char>(std::cout, std::string{"Hello World $%^&"});
    print_ascii_values<wchar_t>(std::wcout, std::wstring{L"Hello World $%^&"});
    // Apparently not supported on the compiler I am using.
    // print_ascii_values<char16_t>(std::basic_ostream<char16_t>, std::u16string_view{u"Hello World $%^&"});
    // print_ascii_values<char32_t>(std::basic_ostream<char32_t>{}, std::u32string_view{U"Hello World"});


    /// 2. Example 2.
    std::string s{"Hello World!!!"};
    reverse_string(s);
    log_me(s);

    // This should give nothing.
    auto end = s.end();
    log_me(*end);

    /// 3. Example 3
    auto trimmed = trim_string("   HELLO WORLD     ");
    log_me("Trimmed:", trimmed,"-");

    /// 4. Example 4
    auto vect = split_string("Hello\nSiddhesh\nHow\nAre\nYou?", '\n');
    log_list(vect);
    return 0;
}
