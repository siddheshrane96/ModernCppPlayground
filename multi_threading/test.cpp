#include <iostream>
#include <type_traits>

// Recursive Tuple Definition
template <typename T, typename... Args>
class Tuple {
public:
    Tuple(const T& value, const Args&... rest)
        : m_value(value), m_rest(rest...) {}

    constexpr size_t size() const { return 1 + m_rest.size(); }

    const T& value() const { return m_value; }
    const Tuple<Args...>& get_rest() const { return m_rest; }

private:
    T m_value;
    Tuple<Args...> m_rest;
};

// Base Case for Tuple
template <typename T>
class Tuple<T> {
public:
    Tuple(const T& value) : m_value(value) {}

    constexpr size_t size() const { return 1; }

    const T& value() const { return m_value; }

private:
    T m_value;
};

// Nth_type: Determine Nth Type
template <size_t N, typename T, typename... Args>
class Nth_type : Nth_type<N - 1, Args...> {
    static_assert(N < sizeof...(Args) + 1, "Index Out of Bounds");

public:
    using value_type = typename Nth_type<N - 1, Args...>::value_type;
};

template <typename T, typename... Args>
class Nth_type<0, T, Args...> {
public:
    using value_type = T;
};

// Get<N>: Fetch Nth Value
template <size_t N>
class Get {
public:
    template <typename T, typename... Args>
    static const typename Nth_type<N, T, Args...>::value_type& get(const Tuple<T, Args...>& tuple) {
        return Get<N - 1>::get(tuple.get_rest());
    }
};

template <>
class Get<0> {
public:
    template <typename T, typename... Args>
    static const T& get(const Tuple<T, Args...>& tuple) {
        return tuple.value();
    }
};

// Helper Function for Get
template <size_t N, typename... Args>
constexpr const typename Nth_type<N, Args...>::value_type& get(const Tuple<Args...>& tuple) {
    return Get<N>::get(tuple);
}

// Main Function: Testing Tuple
int main() {
    Tuple<int, double, char> myTuple(42, 3.14, 'a');

    std::cout << "Size: " << myTuple.size() << '\n';      // Outputs: 3
    std::cout << "First Value: " << get<0>(myTuple) << '\n'; // Outputs: 42
    std::cout << "Second Value: " << get<1>(myTuple) << '\n'; // Outputs: 3.14
    std::cout << "Third Value: " << get<2>(myTuple) << '\n';  // Outputs: a

    return 0;
}
