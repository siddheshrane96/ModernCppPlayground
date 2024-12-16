#include <variant>
#include <vector>
#include <iostream>
#include <cstring>

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

template <typename T, size_t SOO_THRES = 4>
class SmallVectorVariant
{
public:
    SmallVectorVariant() : m_data(StackStorage{})
    {}

    void push_back(const T& value)
    {
        if (std::holds_alternative<StackStorage>(m_data)) 
        {
            auto& stack_storage = std::get<StackStorage>(m_data);
            if (stack_storage.size < SOO_THRES)
            {
                stack_storage.data[stack_storage.size++] = value;
            } 
            else
            {
                // Switch to heap
                HeapStorage heap_storage(SOO_THRES * 2);
                std::copy(stack_storage.data, stack_storage.data + stack_storage.size, heap_storage.data);
                heap_storage.size = stack_storage.size;
                heap_storage.capacity = SOO_THRES * 2;
                heap_storage.data[heap_storage.size++] = value;
                m_data = std::move(heap_storage);
            }
        }
        else
        {
            auto& heap_storage = std::get<HeapStorage>(m_data);
            if (heap_storage.size == heap_storage.capacity)
            {
                heap_storage.grow();
            }
            heap_storage.data[heap_storage.size++] = value;
        }
    }

    T operator[](size_t index) const
    {
        if (std::holds_alternative<StackStorage>(m_data))
        {
            return std::get<StackStorage>(m_data).data[index];
        }
        else
        {
            return std::get<HeapStorage>(m_data).data[index];
        }
    }

    size_t size() const
    {
        if (std::holds_alternative<StackStorage>(m_data))
        {
            return std::get<StackStorage>(m_data).size;
        }
        else
        {
            return std::get<HeapStorage>(m_data).size;
        }
    }

    size_t capacity() const
    {
        if(std::holds_alternative<StackStorage>(m_data))
        {
            return std::get<StackStorage>(m_data).size;
        }
        else
        {
            return std::get<HeapStorage>(m_data).capacity;
        }
    }

private:
    struct StackStorage
    {
        T data[SOO_THRES];
        size_t size = 0;
    };

    struct HeapStorage
    {
        T* data;
        size_t size = 0;
        size_t capacity;

        explicit HeapStorage(size_t cap) : data(new T[cap]), size(0), capacity(cap) {}

        ~HeapStorage() { delete[] data; }

        // Copy constructor (deleted to prevent accidental copies)
        HeapStorage(const HeapStorage&) = delete;
        HeapStorage& operator=(const HeapStorage&) = delete;

        // Move constructor
        HeapStorage(HeapStorage&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity)
        {
            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }

        // Move assignment operator
        HeapStorage& operator=(HeapStorage&& other) noexcept
        {
            if (this != &other)
            {
                delete[] data;
                data = other.data;
                size = other.size;
                capacity = other.capacity;

                other.data = nullptr;
                other.size = 0;
                other.capacity = 0;
            }
            return *this;
        }

        void grow()
        {
            capacity *= 2;
            T* new_data = new T[capacity];
            std::copy(data, data + size, new_data);
            delete[] data;
            data = new_data;
        }
    };

    std::variant<StackStorage, HeapStorage> m_data;
};

int main(int argc, char** argv)
{
    SmallVectorVariant<int> vec;
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

    return 0;
}