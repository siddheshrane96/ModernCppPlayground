#include <iostream>

template <typename T>
class Wrapper
{
public:
    Wrapper(T v):val(v){};
    T val;
};

/**
 *  1.
 *  template <template <typename> typename W>
    class MyTemplate {
        // W is a template that takes one typename parameter
    };

    You can provide a default template for a template template parameter.
    
    2. Template with a default template parameter
    template <template <typename> typename W = FancyWrapper, typename T>
    class UseWrapper {
    public:
        UseWrapper(T v) : wrapped(v) {}
        W<T> wrapped;
    };

    3. Wrapper with multiple arguments
    template <template <typename, typename> typename W, typename T, typename U>
 * 
 */

/**
 * There are two types of template instantiations:
 *  1. Implicit
 *      Unless a template specialization has been explicitly instantiated or explicitly specialized,
 *      the compiler will generate a specialization for the template only when it needs the definition. This is called implicit instantiation.
 *  2. Explicit
 *      A problem that can arise with template instantiations is that you can end up with multiple definitions, one per translation unit.
 *      If the same header that contains a template is included in multiple translation units (.cpp files) and the same template
 *      instantiation is used (let's say wrapper<int> from our previous examples), then identical copies of these instantiations
 *      are put in each translation unit. This leads to increased object sizes.
 *      
 *      The problem can be solved with the help of explicit instantiation declarations, which we will look at next.
 * 
 *      template <typename T>
        struct wrapper
        {
        T data;
        }; 
        extern template wrapper<int>;
        // source1.cpp
        #include "wrapper.h"
        #include <iostream>
        template wrapper<int>;          // [2]
        void f()
        {
        ext::wrapper<int> a{ 42 };
        std::cout << a.data << '\n';
        }
        // source2.cpp
        #include "wrapper.h"
        #include <iostream>
        void g()
        {
        wrapper<int> a{ 100 };
        std::cout << a.data << '\n';
        }
        // main.cpp
        #include "wrapper.h"
        int main()
        {
        wrapper<int> a{ 0 };
        }
 */

template <template <typename> typename W, typename T>
class AnotherWrapper
{
public:
    AnotherWrapper(W<T> wrap): wrapper(wrap){};
    void what_is_val(){
        std::cout << wrapper.val << std::endl;
    }
private:
    W<T> wrapper;
};

int main(int argc, char** argv)
{
    Wrapper<int> wrapper(3);
    AnotherWrapper<Wrapper, int> another(wrapper);
    another.what_is_val();
    return 0;
}