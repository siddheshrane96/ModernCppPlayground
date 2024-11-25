/**
 * @brief My custom implementation of Tuple data structure.
 * 
 **/
#include <iostream>
#include <type_traits>

// Printing helper
template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

// Primary template for the Tuple Class.
template <typename T, typename... Args>
class Tuple
{
public:
    Tuple(const T& val, const Args&... args):
        m_val(val),
        m_rest(args...)
    {}

    constexpr size_t size() const
    {
        return 1 + m_rest.size();
    }

    const T& value() const
    {
        return m_val;
    }

    const Tuple<Args...>& get_rest() const
    {
        return m_rest;
    }

private:
    T m_val;
    Tuple<Args...> m_rest;
};

// Specialized case for the LAST ELEMENT.
template <typename T>
class Tuple<T>
{
public:
    Tuple(const T& val): m_val(val){}

    constexpr size_t size() const
    {
        return 1;
    }

    const T& value() const
    {
        return m_val;
    }

private:
    T m_val;
};


///// Class to figure out Nth type in tuple.
template <size_t N, typename T, typename... Args>
class Nth_type: Nth_type<N-1, Args...>
{
    static_assert(N < sizeof...(Args) + 1, "Index Out of Bounds");
public:
    using value_type = Nth_type<N-1, Args...>::value_type;
};

template<typename T, typename... Args>
class Nth_type<0, T, Args...>
{
public:
    using value_type = T;
};

/// Getter class to fetch Nth element.
template <size_t N>
class Get
{
public:
    template <typename T, typename... Args>
    static const typename Nth_type<N, T, Args...>::value_type& get(const Tuple<T, Args...>& tuple)
    {
        return Get<N-1>::get(tuple.get_rest());
    }
};

template <>
class Get<0>
{
public:
    template <typename T, typename... Args>
    static const T& get(const Tuple<T, Args...>& tuple)
    {
        return tuple.value();
    }
};

// HELPER FUNCTION
template <size_t N, typename... Args>
constexpr const typename Nth_type<N, Args...>::value_type& get(const Tuple<Args...>& tuple)
{
    return Get<N>::get(tuple);
}

int main(int argc, char** argv)
{
    Tuple<int, double> two(1, 2.0);
    LOG(two.size(), " ", two.value());

    // Test Nth type
    static_assert(std::is_same<Nth_type<0,int, double, char>::value_type, int>::value, "Failed");
    static_assert(std::is_same<Nth_type<1,int,double>::value_type, double>::value, "Failed");
    static_assert(std::is_same<Nth_type<2,int,double,char>::value_type, char>::value, "Failed");

    LOG(Get<0>::get(two));
    LOG(Get<1>::get(two));

    Tuple<int, char, float, double> tp(1, 'c', 1.2f, 2.2);
    LOG(get<0>(tp));
    LOG(get<1>(tp));
    LOG(get<2>(tp));
    LOG(get<3>(tp));
    return 0;
}