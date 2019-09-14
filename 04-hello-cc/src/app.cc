#include <iostream>
// extern "C" {
//   #include "calc.h"
// }
extern "C" int add(int, int);

int main() {
  std::cout << "1+2=" << add(2,3) << std::endl;
  return 0;
}