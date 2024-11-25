#include <iostream>
#include <vector>
#include <cassert>

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
std::vector<std::vector<T>> reshape_matrix(
    std::vector<std::vector<T>>& mat, 
    size_t new_rows, 
    size_t new_cols)
{
    assert(mat.size() != 0 && (new_rows*new_cols == mat.size()*mat[0].size()));

    std::vector<std::vector<T>> new_mat(new_rows, std::vector<T>(new_cols));
    /**
     * Let's say matrix has R rows and C cols.
     * Then if we flatten this matrix to 1D, any index k would be mapped from (r,c) as:
     * k = R*r + c
     * 
     * And inversely,
     * r = k/C
     * c = k%C
     */
    int cols_old = mat[0].size();
    for(int i=0; i<new_rows*new_cols; ++i)
    {
        int row_old = i/cols_old;
        int col_old = i%cols_old;

        int row_new = i/new_cols;
        int col_new = i%new_cols;
        new_mat[row_new][col_new] = mat[row_old][col_old];
    }
    return new_mat;
}

int main()
{
    std::vector<std::vector<int>> vect{
        {1,2,3,4},
        {5,6,7,8}
    };
    print_me_2D(vect);
    auto new_vect = reshape_matrix(vect, 4, 2);
    std::cout << "new\n";
    print_me_2D(new_vect);
}