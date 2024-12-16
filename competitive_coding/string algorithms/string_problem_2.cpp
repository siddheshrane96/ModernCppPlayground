#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <optional>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

template <typename T, typename... Args>
void LOG_VEC(T&& arr, Args... args)
{
    LOG(args...);
    for_each(arr.begin(), arr.end(), [](auto a){std::cout << a << " ";});
    std::cout << std::endl;
}

/**
 * 1. Check If Two Strings Are Anagrams: Two strings are anagrams if they contain the same characters with the same frequencies.
 */
bool are_anagrams(const std::string& word1, const std::string& word2)
{
    // Approach1: Using std arrays. For extended ASCII characters use 256 elements.
    std::array<char, 128> fr{0};

    for(auto c: word1)
        fr[static_cast<int>(c)] += 1;
    for(auto c: word2)
        fr[static_cast<int>(c)] -= 1;

    for(auto c: fr)
    {
        if(c != 0)
            return false;
    }

    // Approach 2: Using unordered_map
    // std::unordered_map<char, int> map;
    // for(auto c: word1)
    //     map[c] += 1;
    
    // for(auto c:word2)
    // {
    //     if(--map[c] < 0)
    //     {
    //         return false;
    //     }
    // }
    return true;
}

/**
 * Example2: Check If Two Strings Have the Same Unique Characters.
 * Definition: Two strings have the same unique characters if their sets of characters are identical.
 */
bool same_chars(const std::string& word1, const std::string& word2)
{
    std::array<bool, 128> s1{false};
    std::array<bool, 128> s2{false};
    for(char c: word1)
        s1[static_cast<int>(c)] = true;
    for(char c: word2)
        s2[static_cast<int>(c)] = true;

    return s1 == s2; // same can be done for std::unordered_set ( == operator would for it too).
}

/**
 *  Example3: Find Common Characters Between Two Strings (NON UNIQUE).
    Definition: Return a string containing the characters that appear in both strings.
 */
std::string find_common_chars(const std::string& word1, const std::string& word2)
{
    std::string common{};
    std::array<int, 128> s1{0};
    std::array<int, 128> s2{0};
    for(char c: word1)
        s1[static_cast<int>(c)] += 1;
    for(char c: word2)
        s2[static_cast<int>(c)] += 1;

    for(int i=0; i<128; ++i)
    {
        int common_count = std::min(s1[i], s2[i]);
        common += std::string(common_count, static_cast<char>(i));
    }
    return common;
}

/**
 * Example 4: Check If One String Is a Subsequence of Another.
 * Definition: A string is a subsequence of another il7f all characters in the first string appear in the second string in the same order.
 * NOTE: This is SUBSEQUENCE and not SUBSTRING.
 */
bool is_subsequence_of(const std::string& key, const std::string& word)
{
    size_t i=0;
    size_t j=0;
    while(i < key.size() && j < word.size())
    {
        if(key[i] == word[j])
            ++i;
        ++j;
    }
    return i == key.size();
}

/**
 * Example 5: Split a string by delimeter.
 */
std::vector<std::string> split_by_delem(const std::string& str, char delim)
{
    std::vector<std::string> ans;
    auto start = 0;
    auto end = start;
    while(end != std::string::npos)
    {
        end = str.find_first_of(delim, start);
        ans.push_back(str.substr(start,end - start));
        start = end+1;
    }
    return ans;
}

/**
 * Example 6: Split a string by space.
 */
std::vector<std::string> split_by_space(const std::string& str)
{
    std::vector<std::string> ans;
    std::stringstream ss(str);
    std::string word;
    while(ss >> word)
    {
        ans.push_back(word);
    }
    return ans;
}

/**
 * Example 7: Reverse a statement.
 */
std::string reverse_a_statement(const std::string& str)
{
    auto vect = split_by_space(str);
    std::stringstream ss;
    for(int i=vect.size()-1; i>=0;--i)
    {
        if(i!=vect.size()-1)
            ss << " ";
        ss << vect[i];
    }
    return ss.str();
}

/**
 * Example 8: Longest word in a statement.
 */
std::string find_longest_word(const std::string& statement)
{
    std::string longest_word;
    std::stringstream ss(statement);
    std::string word;
    while(ss >> word)
    {
        if(word.size() > longest_word.size())
            longest_word = word;
    }
    return longest_word;
}

/**
 * Example 9: Replace All Occurrences of a Substring.
 * Write a function to replace all occurrences of a substring with another substring in a string.
 */
std::string replaceAllOccurrences(std::string str, const std::string& from, const std::string& to)
{
    size_t start = 0;
    while( ((start = str.find(from, start)) != std::string::npos))
    {
        LOG(start);
        str.replace(start, from.length(), to);
        start += to.length();
    }
    return str;
}

/**
 * Example 10: Convert a String to an Integer.
 * Write a function to convert a string to an integer, handling possible errors.
 */
std::optional<int> stringToInteger(const std::string& str) 
{
    size_t pos;
    try
    {
        int i= std::stoi(str, &pos);
        return i;
    }
    catch(const std::invalid_argument& e)
    {
        LOG("Invalid Argument exception ", e.what());
    }
    catch(const std::out_of_range& e)
    {
        LOG("Out of Range exception ", e.what());
    }
    return std::nullopt;
}

int main()
{   
    // Example1:
    LOG(are_anagrams("Hello", "Hlleo"));
    LOG(are_anagrams("Helllo", "Hlleo"));

    LOG(find_common_chars("Helloooo","llooSSS"));

    LOG(is_subsequence_of("abcd","aeeeebrrfrfcbvbbdsdda"));

    auto vec = split_by_delem("Hello-World-Siddhesh", '-');
    LOG_VEC(vec, "Split by", '-');
    
    vec = split_by_space("This is spartaaaaa");
    LOG_VEC(vec, "Split by"," space ");
    
    auto ss = reverse_a_statement("Hello this is C++ program!");
    LOG(ss);

    LOG(find_longest_word("Hi I am Siddhesh Rane, a very succesfull computer/robotics scientist."));
    
    auto replaced = replaceAllOccurrences("This is naive string with lots of naive words, naive statements.", "naive", "cool");
    LOG(replaced);

    auto maybe_int = stringToInteger("123");
    auto maybe_int2 = stringToInteger("a123ab");
    LOG(*maybe_int);
    return 0;
}