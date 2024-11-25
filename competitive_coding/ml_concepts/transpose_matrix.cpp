#include <iostream>
#include <vector>

template <typename T>
void print_me(T&& arr)
{
    std::for_each(arr.begin(), arr.end(), [](auto& a) { std::cout << a << " "; });
    std::cout << std::endl;
}

template <typename T>
void print_me_2D(T&& arr)
{
    std::for_each(arr.begin(), arr.end(), [](auto& a) { print_me(a); });
}


template<typename T>
std::vector<std::vector<T>> get_transpose(std::vector<std::vector<T>>& mat)
{
    size_t rows = mat.size();
    size_t cols = mat[0].size();

    std::vector<std::vector<T>> mat_T(rows, std::vector<T>(cols,0));
    for(size_t row=0; row<rows;++row)
    {
        for(size_t col=0; col<cols;++col)
        {
            mat_T[row][col] = mat[col][row];
        }
    }
    return mat_T;
}

int main()
{
    std::vector<std::vector<int>> vect{
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    auto vect_t = get_transpose(vect);
    print_me_2D(vect_t);
    return 0;
}