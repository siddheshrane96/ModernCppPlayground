#include <iostream>

auto allocated = size_t{0}; 
void* operator new(size_t size) {  
  void* p = std::malloc(size); 
  allocated += size; 
  return p; 
} 
 
void operator delete(void* p) noexcept { 
  return std::free(p); 
} 
 
int main() { 
  allocated = 0; 
  auto s = std::string{"123456789123456"}; 
  std::cout << "For 15 chars\n";
  std::cout << "stack space = " << sizeof(s) 
    << ", heap space = " << allocated 
    << ", capacity = " << s.capacity() << '\n';

  allocated = 0;
  auto s2 = std::string{"1234567891234567"};
  std::cout << "For 16 chars\n";
  std::cout << "stack space = " << sizeof(s2) 
    << ", heap space = " << allocated 
    << ", capacity = " << s2.capacity() << '\n'; 
}
