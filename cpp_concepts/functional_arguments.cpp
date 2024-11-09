#include <iostream>
#include <functional>

/**
 * 1. Function pointers
 *      Functions pointers have syntax: return_type(*function_name)(parameterTypes)
 */
void executer(void (*function_ptr)(int), int arg)
{
    function_ptr(arg);
}

int executer_return(int (*function_ptr)(int), int arg)
{
    return function_ptr(arg);
}

void print_me(int i)
{
    std::cout << "Int passed " << i << std::endl;
}

int return_me(int i)
{
    return i;
}

/**
 * 2. Function pointers in template arguments.
 */
template <void (*func_ptr)()>
struct Greeter
{
    void greet()
    {
        func_ptr();
    }
};

void say_hello() { std::cout << "Hello World English!\n"; }
void say_konichiwa() { std::cout << "Konichiwa Japan!\n"; }

/**
 * 3. Using std::function
 * It has the syntax:  std::function<ReturnType(ParameterTypes)>
 */
template <typename T>
T executor_new(std::function<T(T)> func, T input)
{
    return func(input);
}

float square_me(float num)
{
    std::cout << "Squared num: " << num*num << std::endl;
    return num*num;
}

/**
 * 4a. Passing member function as parameter.
 *      The syntax is ReturnType (ClassName::*funcPtr)(ParameterTypes)
 */
class Display
{
public:
    void print(int val)
    {
        std::cout << "Val: " << val << std::endl;
    }
};

void example_exacution()
{
    // Let's call above function without std::function
    Display obj;
    void (Display::*funcPtr)(int) = &Display::print;
    (obj.*funcPtr)(10);
}

/**
 * 4b. Using std::bind. It makes it easier to pass around and call member functions.
 *      Syntax is: std::bind(&ClassName::Function, instance, placeholders...)
 */
class DummyClass2
{
public:
    void print_num(int num)
    {
        std::cout << "I got " << num << std::endl;
    }
};

void execute_bind(std::function<void(int)> func, int num)
{
    std::cout << "Calling from bind ";
    func(num);
}

/**
 * 5. Functors are objects that act like functions by overloading operator().
 *      struct Functor
 *      {
            void operator()(int x) const {
                std::cout << "Functor called with: " << x << std::endl;
            }
        };

        and called be like
        Functor f;
        f(19);
 */

int main(int agrc, char** argv)
{
    // example1
    executer(&print_me, 3);
    int a = executer_return(&return_me, 3);
    std::cout << "Returned " << a << std::endl;

    // example2
    Greeter<&say_hello> english;
    Greeter<&say_konichiwa> japanese;
    english.greet();
    japanese.greet();

    // example3
    executor_new<float>(square_me, 3.2f);
    auto lamb = [](int a){ 
        std::cout << "Int squarer is called " << a*a << std::endl;
        return a*a;
    };
    executor_new<int>(lamb, 4);

    // example 4a
    example_exacution();

    // example 4b
    DummyClass2 d2;
    auto d_func = std::bind(&DummyClass2::print_num, d2, std::placeholders::_1);
    execute_bind(d_func, 20);
    return 0;
}