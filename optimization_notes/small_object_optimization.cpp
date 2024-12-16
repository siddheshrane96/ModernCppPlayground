#include <vector>
#include <iostream>
#include <cstring>

/**
 * NOTES:
 * 1. void* void* is a pointer type that can hold the address of any type of object, making it a "generic" pointer. 
 * It is often used in low-level programming for situations where the exact type of the data being pointed to is
 * not known at compile time.
 * Use Cases for void*:

    1. Generic Pointers: void* allows you to create pointers that are not type-specific. For example:

    int x = 42;
    void* ptr = &x; // ptr can point to any type

    2. Interfacing with C Libraries: Many C APIs (e.g., malloc and free) use void* to work with data of any type. For example:

    int* arr = (int*)malloc(5 * sizeof(int)); // malloc returns void*
    free(arr);

    Passing Raw Memory: It is useful when passing raw memory or generic objects, such as in custom memory allocators.
 * Important Characteristics of void*:

    Cannot Be Dereferenced Directly: Since void* does not point to a specific type, dereferencing it is not allowed because
    the compiler does not know the size or type of the object being pointed to.
    ~
    void* ptr = &x;
    std::cout << *ptr; // Error! Cannot dereference void*
    ~
    To dereference, you must cast it to the appropriate pointer type:
    ~
    void* ptr = &x;
    std::cout << *(int*)ptr; // Cast to int* before dereferencing
    ~
    
    Cannot Perform Pointer Arithmetic: Pointer arithmetic (e.g., ptr++, ptr + 1) requires knowledge of the data type's size, which is not possible with void*.
    Size and Alignment Are Not Defined: A void* pointer can hold the address of any type, but the size of the object being pointed to is unknown.
 */

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

size_t heap_allocations = 0;
void* operator new(size_t size)
{
    heap_allocations += size;
    return std::malloc(size);
}

void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}

template <typename T>
class SmallVector
{
public:
    SmallVector(size_t capacity = 4): 
        m_capacity(capacity),
        m_size(0),
        m_using_heap{false}
    {
    }

    ~SmallVector()
    {
        if(m_using_heap) delete[] m_heap_data;
    }

    void push_back(int value)
    {
        if(m_size < SOO_THRES)
        {
            m_stack_data[m_size++] = value;
        }
        else
        {
            if(!m_using_heap || m_size == m_capacity)
            {
                grow();
            }
            m_heap_data[m_size++] = value;
        }
    }

    void grow()
    {
        m_capacity *= 2;
        T* m_new_heap_data = new T[m_capacity];
        if(!m_using_heap)
        {
            std::memcpy(m_new_heap_data, m_stack_data, sizeof(T)* m_size);
            m_using_heap = true;
        }
        else
        {    
            std::memcpy(m_new_heap_data, m_heap_data, sizeof(T)* m_size);
            delete[] m_heap_data;
        }
        m_heap_data = m_new_heap_data;
    }

    T operator[](size_t index) const
    {
        if(!m_using_heap)
        {
            return m_stack_data[index];
        }
        else
        {
            return m_heap_data[index];
        }
    }

    size_t size() const
    {
        return m_size;
    }

    size_t capacity() const
    {
        return m_capacity;
    }

private:
    size_t m_capacity;
    size_t m_size;
    bool m_using_heap;
    static constexpr size_t SOO_THRES=4;
    union {
        alignas(T) T m_stack_data[SOO_THRES];
        T* m_heap_data;
    };
};

int main(int argc, char** argv)
{
    SmallVector<int> vec;
    for (int i = 0; i < 4; ++i) vec.push_back(i); // Stack storage
    LOG("Heap allocations after first 4 push: ", heap_allocations, " size ", vec.size(), " capacity ", vec.capacity());
    vec.push_back(5); // Switch to heap storage
    LOG("Heap allocations after 5th push: ", heap_allocations, " size ", vec.size(), " capacity ", vec.capacity());
    vec.push_back(6);
    LOG("Heap allocations after 6th push: ", heap_allocations, " size ", vec.size(), " capacity ", vec.capacity());
    vec.push_back(7);
    LOG("Heap allocations after 7th push: ", heap_allocations, " size ", vec.size(), " capacity ", vec.capacity());
    vec.push_back(8);
    LOG("Heap allocations after 8th push: ", heap_allocations, " size ", vec.size(), " capacity ", vec.capacity());
    vec.push_back(9);
    LOG("Heap allocations after 9th push: ", heap_allocations, " size ", vec.size(), " capacity ", vec.capacity());

    /**
     * Why do you see output 16 48 48 112?
     * First Push Beyond Capacity:

        When push_back(5) is called, m_size == m_capacity (both are 4).
        Capacity is doubled to 8.
        A new array of size 8 is allocated (new int[8] → 32 bytes).
        Old array of size 4 is deallocated, but this does not affect the heap_allocations counter.
        Total heap allocations: 16 + 32 = 48.
     */
    return 0;
}