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

enum class MAJOR{
    ROW,
    COL
};

template <typename T>
std::vector<float> average(const std::vector<std::vector<T>>& mat, MAJOR major)
{
    std::vector<float> avg_vect;
    if(major == MAJOR::ROW)
    {
        for(size_t i=0; i<mat.size(); ++i)
        {
            float sum = 0;
            for(size_t j=0; j<mat[i].size(); ++j)
                sum += mat[i][j];
            avg_vect.push_back(sum/mat[i].size());
        }
    }
    else
    {
        for(size_t j=0; j<mat[0].size(); ++j)
        {
            T sum = 0;
            for(size_t i=0; i<mat.size(); ++i)
                sum += mat[i][j];
            avg_vect.push_back(sum/mat.size());
        }
    }
    return avg_vect;
}

int main()
{
    std::vector<std::vector<int>> vect{
        {1,2,3,4},
        {5,6,7,8}
    };
    print_me_2D(vect);
    auto row = average(vect, MAJOR::ROW);
    std::cout << "row\n";
    print_me(row);

    auto col = average(vect, MAJOR::COL);
    std::cout << "col\n";
    print_me(col);
}