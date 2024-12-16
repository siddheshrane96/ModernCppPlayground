#include <iostream>

void function_array_pointer(int* arr, int size)
{
    // int* p = arr;
    // for(int i=0;i<size;++i)
    //     std::cout << *(p+i) << " ";
    for(int i=0;i<size;++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

template<typename T, size_t N>
void function_array_reference(const T(&arr)[N])
{
    for(size_t i=0; i<N;++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

// When passing a multidimensional array, you must specify sizes for all but the first dimension.
// ONLY WORKS FOR STATIC ARRAY
template <size_t rows, size_t cols>
void print_2D_static_array(const int (&mat)[rows][cols])
{
    std::cout << "static 2d " << std::endl;
    for(size_t i=0;i<rows;++i)
    {
        for(size_t j=0;j<cols;++j)
        {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void fill_zeros(int** mat, int rows, int cols)
{
    for(size_t i=0;i<rows;++i)
    {
        for(size_t j=0;j<cols;++j)
        {
            mat[i][j] = 0;
        }
    }
}

void print_2D_dynamic_array(int** mat, int rows, int cols)
{
    for(size_t i=0;i<rows;++i)
    {
        for(size_t j=0;j<cols;++j)
        {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}



int main()
{
    // 1.1 Single-Dimensional Arrays
    int a[3];

    // 1.2 Initializing at Declaration
    int arr1[5] = {1, 2, 3, 4, 5};  // Fully initialized
    int arr2[5] = {1, 2};           // Partially initialized (remaining elements = 0)
    int arr3[] = {1, 2, 3};         // Compiler deduces size (3 in this case)

    // 1.3 Using new for Dynamic Arrays
    int* arr4 = new int[5]; // Creates a dynamic array of size 5
    delete[] arr4;

    // 1.4 Array names decay into pointers
    int arr5[5] = {10, 20, 30, 40, 50};
    int* p = arr5; // Points to the first element

    int second = *(p+1); //Pointer arithmetic
    *p = 1; // modifies first element
    *(p+1) = 3; // modifies third element

    int a2[5] = {1,2,3,4,5};
    function_array_pointer(a2, 5);
    function_array_pointer(a2, 3);
    function_array_pointer(a2, 7); // Should give random values after size limit is reached.
    function_array_reference<int, 5>(a2);

    // 1.5 Creating static 2D array and passing to a function.
    int mat[2][3] = {{1,2,3}, {4,5,6}};
    print_2D_static_array(mat);

    // 1.6 Creating dynamic 2D array
    int** mat2 = new int*[2]; // 2 rows are created
    for(size_t i=0; i<2;++i)
    {
        mat2[i] = new int[3];
    }
    fill_zeros(mat2, 2,3);
    print_2D_dynamic_array(mat2,2,3);

    // Clearing memory
    for(size_t i=0; i<2;++i)
    {
        delete[] mat2[i];
    }
    delete[] mat2;
    return 0;
}