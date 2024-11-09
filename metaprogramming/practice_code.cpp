#include <iostream>
#include <vector>
#include <string>
/**
 * INTRODUCTION
 * 
 * * Templates have three types of parameters:
 *      1. type parameters, eg template <typename T>
 *      2. non-type parameters, eg template <size_t N>
 *      3. template template-type eg template<typename K, typename V, template<typename> typename C>
 * 
 * 
 */

/**
 * Example1: Custom implementation of count_if.
 */
template <typename Iterator, typename Predicate>
size_t custom_count_if(Iterator start, Iterator end, Predicate p)
{
    size_t count = 0;
    for(auto i=start; i!=end; ++i)
    {
        if(p(*i))
            count+=1;
    }
    return count;
}

/**
 * Example2: Dummy class with different classs and member template parameter.
 */
template <typename T>
class Dummy
{
public:
    Dummy(T val): m_val(val){}
    T get_val() { return m_val; }

    template <typename U>
    U get_val_as() {
        return static_cast<U>(m_val);
    }
private:
    T m_val;
};

/**
 * Example3: Template with value parameter
 */
template <typename T, size_t size=5>
class Buffer
{
private:
    T m_data[size];
public:
    constexpr  T* data() const {
        return m_data;
    }

    constexpr const T& operator[](size_t index) const
    {
        static_assert(index > 0 && index < size);
        return m_data[index];
    }

    constexpr T& operator[](size_t index)
    {
        static_assert(index > 0 && index < size);
        return m_data[index];
    }

    constexpr size_t get_size(){
        return size;
    }
};

/**
 * Example4: Passing function as a template parameter.
 */

void polite_greet(const std::string& str)
{
    std::cout << "Hello wonderful " << str << std::endl;
}

void rude_greet(const std::string& str)
{
    std::cout << "Hello stupid " << str << std::endl;
}

class BaseDevice
{
public:
    virtual void greet() = 0;
    virtual ~BaseDevice() = default;
};

template <void (*funct_ptr)(const std::string&)>
class SmartDevice: public BaseDevice
{
public:
    SmartDevice(std::string name):
        m_name(name){}

    void greet() override
    {
        funct_ptr(m_name);
    }

private:
    std::string m_name;
};

int main(int argc, char** argv)
{
    std::cout << "Hello World!" << std::endl;

    // Example1
    std::vector<int> arr{1,2,3,4,5,6,7};
    auto even_count = custom_count_if(arr.begin(), arr.end(),
            [](int& a){
                return (a&1)==0;
            });
    std::cout << "Even count " << even_count << std::endl;

    // Example2
    Dummy<float> num(42.434f);
    std::cout << num.get_val() << "->" << num.get_val_as<int>() << std::endl;

    // Example3
    Buffer<float> buff;
    std::cout << buff.get_size() << std::endl;

    // Example4
    SmartDevice<&polite_greet> s_device("Siddhesh");
    s_device.greet();
    SmartDevice<&rude_greet> s_device2("Siddhesh");
    s_device2.greet();
    
    return 0;
}