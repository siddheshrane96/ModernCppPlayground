#include <iostream>
#include <vector>
#include <algorithm>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args ) << std::endl;
}

/**
 * Example 1: A function make_vector() that mimics methods like std::make_pair std::make_unique etc.
 */
template <typename T, typename... Args, typename Allocator = std::allocator<T>>
std::vector<T, Allocator> make_vector(T first, Args... args)
{
    return std::vector<std::decay_t<T>, Allocator> {
        std::forward<T>(first),
        std::forward<T>(args)...
    };
}

/**
 * Example 2: Perfect forwarding tutorial FAILURE.
 */
void h(int& v)
{
    LOG("Called h lval");
}
void h(int&& v)
{
    LOG("Called h rval");
}

void g(int& v)
{
    h(v);
}
void g(int&& v)
{
    // Inside g() v itself is lvalue.
    // So should be called with h(std::forward<int&&>(v));
    h(v); // WRONG
    // h(std::forward<int&&>(v)); // RIGHT
}

/**
 * NOTE: FORWARDING
 * template <typename T>
 * void f(T&& arg); // This is forwarding reference.
 *  
 * But const T&& or std::vector<T>&& 
 * or S::f in fillowoing Struct are NOT FORWARDING references. They are rvalue.
 * struct S
 * {
 *  void f(T&& arg) { std::cout << "S.f(T&&)\n"; }
 * }
 * 
 *  
 * The && in the first overload does not necessarily mean an rvalue reference. It means an rvalue reference if an rvalue 
 * was passed or an lvalue reference if an lvalue was passed. Such a reference is called a forwarding reference.
 * However, forwarding references are only present in the context of an rvalue reference to a template parameter. 
 * It has to have the form T&& and nothing else. T const&& or std::vector<T>&& are not forwarding references, but normal rvalue references.
 * Similarly, the T&& in the f function member of the class template S is also an rvalue reference because f is not a template but
 * a non-template member function of a class template, so this rule for forwarding references does not apply.
 * 
 */

// Perfect forwarding.
template <typename T>
void H(T& val)
{
    LOG("H LVAL");
}
template <typename T>
void H(T&& val) // NOTE THIS IS ALSO FORWADING REFERENCE
{
    LOG("H RVAL");
}

template <typename T>
void G(T&& val)
{
    H(std::forward<T>(val));
}


int main(int argc, char** argv)
{
    auto my_vec = make_vector(1,2,3,4,5,6);
    std::for_each(my_vec.begin(), my_vec.end(), [](auto a){ std::cout << a << " ";});
    std::cout << std::endl;
    
    // Forwarding failure example.
    int x = 2;
    g(x); // both results in h lval
    g(3); // both results in h lval
    
    G(x);
    G(3);
    return 0;
}