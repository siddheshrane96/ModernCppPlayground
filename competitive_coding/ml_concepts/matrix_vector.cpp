#include <iostream>
#include <vector>
#include <array>
/**
 * Write a function that calculates the dot product of a matrix and a vector. return -1 if the matrix could not be dotted with the vector
 * Example:
        input: a = [[1,2],[2,4]], b = [1,2]
        output:[5, 10] 
        reasoning: 1*1 + 2*2 = 5;
                   1*2+ 2*4 = 10
 */
template <typename T>
void print_me(T&& arr)
{
    for(auto& a: arr)
        std::cout << a << " ";
    std::cout << std::endl; 
}

template <size_t rows, size_t cols, typename N=int>
std::array<N, rows> dot_product_mat_vec(std::array<std::array<N, cols>, rows>& mat, std::array<N, cols>& vec)
{
    std::array<N, rows> ans({0});
    for(size_t i=0; i<rows; ++i)
    {
        for(size_t j=0; j<cols; ++j)
        {
            ans[i] += mat[i][j] * vec[j];
        }
    }
    
    return ans;
}

int main(int argc, char** argv)
{
    std::array<std::array<int,2>,2> a{
        {{1,2},{2,4}}
    };
    std::array<int, 2> b{1,2};
    auto ans = dot_product_mat_vec(a,b);
    print_me(ans);
    return 0;
}

/**
 * 
 * MORE GENERIC CONTAINER CONCEPTS. @todo Learn in depth later on:

#include <type_traits>
#include <vector>
#include <array>

// Concept to detect a 1D container (like std::vector<int> or std::array<int, N>)
template <typename T>
concept OneDContainer = requires(T a) {
    typename T::value_type;                     // T has a value_type
} && !requires(typename T::value_type v) {      // T's value_type does not have a nested value_type (not a container of containers)
    typename T::value_type::value_type;
};

// Concept to detect a 2D container (like std::vector<std::vector<int>> or std::array<std::array<int, N>, M>)
template <typename T>
concept TwoDContainer = requires(T a) {
    typename T::value_type;                     // T has a value_type
    typename T::value_type::value_type;         // T's value_type also has a value_type (it's a container of containers)
};

 */