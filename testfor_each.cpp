// mem_fn example
#include <iostream>     // std::cout
#include <functional>   // std::mem_fn
#include <thread>
<<<<<<< HEAD
#include <vector>
=======
>>>>>>> 9f5d9a9d6ab4c6789e794a646e80d1cc52cace93

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
<<<<<<< HEAD
  //std::distance:通过迭代器，统一算具有迭代器容器的个数
  std::vector<int> Vec{1,2,3};
  int length = std::distance(Vec.begin(),Vec.end());
  int Size = Vec.size();
  std::cout << "Vec length:" << Size << std::endl;
  std::cout << "this thread id :" << std::this_thread::get_id() << std::endl;
=======
>>>>>>> 9f5d9a9d6ab4c6789e794a646e80d1cc52cace93
  return 0;
}