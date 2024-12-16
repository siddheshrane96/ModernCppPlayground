#include <iostream>
#include <string>
#include <optional>
#include <cctype>
#include <cassert>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

/**
 * Example 11: Implement a Basic String Compression Algorithm
 * Write a function that compresses a string using the counts of repeated characters.
 * For example, the string "aabcccccaaa" would become "a2b1c5a3". 
 * If the compressed string is not smaller than the original, return the original string.
 */
std::string get_compressed_string(const std::string& str)
{
    std::string ret_str;
    for(size_t i=0; i<str.size(); ++i)
    {
        auto end = i;
        while(str[i] == str[end])
        {
            ++end;
        }
        ret_str += str[i] + std::to_string(end-i);
        i = end-1;
    }
    return (ret_str.size() < str.size())? ret_str : str;
}

/**
 * Example 12: Implement a Basic String Decompression Algorithm.
 * Write a function that decompresses a string that was compressed using the above method.
 * For example, the string "a2b1c5a3" would become "aabcccccaaa".
 */
std::optional<std::string> get_decompressed_string(const std::string& str)
{
    std::string ret_str{""};
    if(str.empty() || std::isdigit(str[0]))
    {
        LOG("INVALID INPUT");
        return std::nullopt;
    }

    for(size_t curr=0; curr<str.size(); ++curr)
    {
        char curr_char = str[curr];
        size_t end = curr+1;
        while(end < str.size() && std::isdigit(str[end]))
        {
            ++end;
        }
        std::string count_str = str.substr(curr+1, end-curr-1);
        int count = std::atoi(count_str.c_str());
        
        ret_str.append(count,curr_char);
        curr = end-1;
    }
    return ret_str;
}

int main(int argc, char** argv)
{
    LOG("Compressed ", get_compressed_string("aaaaaabbbccc"));
    
    std::optional<std::string> maybe_dec = get_decompressed_string("a2b13c4f11");
    if(maybe_dec)
        LOG("Decompressed ", *maybe_dec);
    return 0;
}