#include <iostream>

/**
 * NOTE: DECLTYPE Rules
 * The decltype specifier is not restricted for use in template code. It can be used with different expressions, and it yields different results based on the expression. The rules are as follows:

    1. If the expression is an identifier or a class member access, then the result is the type of the entity that is named by the expression. If the entity does not exist, or it is a function that has an overload set (more than one function with the same name exists), then the compiler will generate an error.
    2. If the expression is a function call or an overloaded operator function, then the result is the return type of the function. If the overloaded operator is wrapped in parentheses, these are ignored.
    3. If the expression is an lvalue, then the result type is an lvalue reference to the type of expression.
    4. If the expression is something else, then the result type is the type of the expression.
 */

// To understand these expressions, Let's create some examples.
int f() { return 42; }
int g() { return 0; }
int g(int a) { return a; }
struct wrapper
{
   int val;
   int get() const { return val; }
};
int a = 42;
int& ra = a;
const double d = 42.99;
long arr[10];
long l = 0;
char* p = nullptr;
char c = 'x';
wrapper w1{ 1 };
wrapper* w2 = new wrapper{ 2 };

/**
 * EXAMPLE: Perfect forwarding of return type
 */
template <typename T>
T const& func(T const& ref)
{
   return ref;
}

template <typename T>
/// NOTE: If you had used only auto as return type, it will be evaluated as int.
decltype(auto) func_caller(T&& ref)
{
   return func(std::forward<T>(ref));
}
// int a = 42;
// decltype(func(a))        r1 = func(a);        // int const&
// decltype(func_caller(a)) r2 = func_caller(a); // int const&


/**
 * EXAMPLE: Why typename is required?
 * "A name used in a template declaration or definition and that is dependent on a template-parameter
 *  is assumed not to name a type unless the applicable name lookup finds a type name or the name
 *  is qualified by the keyword typename."
 *  C++ standard defines a "two-phase name lookup" rule for names in templates. Names inside templates are divided to two types:

    1. Dependent - names that depend on the template parameters but aren't declared within the template.
    2. Non-dependent - names that don't depend on the template parameters, plus the name of the template itself and names declared within it.

    When the compiler tries to resolve some name in the code, it first decides whether the name is dependent or not, and the resolution process
    stems from this distinction. While non-dependent names are resolved "normally" - when the template is defined, the resolution for dependent
    names happens at the point of the template's instantiation. This is what ensures that a specialization can be noticed correctly in the example above.
 */
template <typename T>
struct Base {
   typedef int MyType;
};

template <typename T>
struct Derived : Base<T> {
   void g() {
       // A. error: ‘MyType’ was not declared in this scope
       // MyType k = 2;

       // B. error: need ‘typename’ before ‘Base<T>::MyType’ because
       // ‘Base<T>’ is a dependent scope
       // Base<T>::MyType k = 2;

       // C. works!
       typename Base<T>::MyType k = 2;

       std::cerr << "Derived<T>::g --> " << k << "\n";
   }
};


int main()
{
    decltype(a) e1;             // R1, int
    decltype(ra) e2 = a;        // R1, int&
    decltype(f) e3;             // R1, int()
    decltype(f()) e4;           // R2, int
    // decltype(g) e5;             // R1, error (‘decltype’ cannot resolve address of overloaded function)
    decltype(g(1)) e6;          // R2, int
    decltype(&f) e7 = nullptr;  // R4, int(*)()
    decltype(d) e8 = 1;         // R1, const double
    decltype(arr) e9;           // R1, long[10]
    decltype(arr[1]) e10 = l;   // R3, long&
    decltype(w1.val) e11;       // R1, int
    decltype(w1.get()) e12;     // R1, int
    decltype(w2->val) e13;      // R1, int
    decltype(w2->get()) e14;    // R1, int
    decltype(42) e15 = 1;       // R4, int
    decltype(1 + 2) e16;        // R4, int
    decltype(a + 1) e17;        // R4, int
    decltype(a = 0) e18 = a;    // R3, int&
    decltype(p) e19 = nullptr;  // R1, char*
    decltype(*p) e20 = c;       // R3, char&
    decltype(p[0]) e21 = c;     // R3, char&

    /**
    decltype(f) only names a function with an overloaded set, so rule 1 applies. decltype(g) also names a function but it has an overloaded set. 
    Therefore, rule 1 applies and the compiler generates an error.
    
    decltype(f()) and decltype(g(1)) are both using function calls for the expression, so the second rule applies,
    and even if g has an overload set, the declaration is correct.
    
    decltype(&f) uses the address of the function f, so the fourth rule applies, yielding int(*)().
    
    decltype(1+2) and decltype(a+1) use the overloaded operator + that returns an rvalue, so the fourth rule applies.
    The result is int. However, decltype(a = 1) uses the assignment operator that returns an lvalue, so the third rule applies, yielding the lvalue reference int&.
     */
    return 0;
}