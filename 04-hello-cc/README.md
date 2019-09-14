app.cc
```
#include <iostream>

int main() {
  std::cout << "hello world\n";
  return 0;
}
```

compile:
`g++ app.cc -o app`

using gcc
`gcc app.cc -lstdc++ -o app`

[gcc和g++是什么关系？ - 知乎](https://www.zhihu.com/question/20940822)

增加对libcalc.so的引用，
app.cc
```
include <iostream>
extern "C" {
  #include "calc.h"
}

int main() {
  std::cout << "1+2=" << add(2,3) << std::endl;
  return 0;
}
```

compile & link:
`g++ -L/Users/nonocast/lib -lcalc app.cc -o app`

```
otool -L app
app:
        libcalc.so (compatibility version 0.0.0, current version 0.0.0)
        /usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 800.6.0)
        /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1279.0.0)
```

同等的，不需要.h也是同样一个意思，
```
#include <iostream>
extern "C" int add(int, int);

int main() {
  std::cout << "1+2=" << add(2,3) << std::endl;
  return 0;
}
```

其实关键是调用函数的签名, 能link就行, 仔细体会。

