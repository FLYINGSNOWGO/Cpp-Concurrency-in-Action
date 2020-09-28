// mem_fn example
#include <iostream>     // std::cout
#include <functional>   // std::mem_fn
#include <thread>

class A {
public:
  int value;
  int triple() {return value*3;}
  void print()
  {
      std::cout << "123" << std::endl;
  }
};

int main () {
  A five {5};

  // 直接调用成员函数
  std::cout << five.triple() << std::endl;

  // 使用mem_fn调用
  auto triple = std::mem_fn (&A::triple);
  std::cout << triple(five) << std::endl;

  auto p = std::mem_fn(&A::print);
  p(five);
  std::cout << "===>:" << std::thread::hardware_concurrency() << std::endl;
  return 0;
}