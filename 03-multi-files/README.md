gcc app.c 包括4个步骤:
- Prepressing (预处理)
- Compilation (编译)
- Assembly (汇编)
- Linking (链接)

Prepressing (预编译)
-------------------
$gcc -E app.c -o app.i

gcc实际是一个工具链，针对不同的语言有不同的套路，如果是c语言则采用cpp来做真正的预编译，所以效果等同于调用,

$cpp hello.c > hello.i

假定app.c
```
int main(void) {
  return 0;
}
```
得到的结果是app.i
```
# 1 "app.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 362 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "app.c" 2
int main(void) {
  return 0;
}
```

加上stdio.h的app.c
```
#include <stdio.h>

int main(void) {
  printf("hello world\n");
  return 0;
}
```

这样生成app.i就有544行，
```
# 1 "app.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 362 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "app.c" 2
# 1 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/stdio.h" 1 3 4
# 64 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/stdio.h" 3 4
# 1 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/_stdio.h" 1 3 4
# 68 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/_stdio.h" 3 4
# 1 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/sys/cdefs.h" 1 3 4
# 626 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/sys/cdefs.h" 3 4
...
# 1 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/i386/_types.h" 1 3 4
# 37 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/i386/_types.h" 3 4
typedef signed char __int8_t;

typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef long long __int64_t;
typedef unsigned long long __uint64_t;

...

extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;
# 142 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/stdio.h" 3 4
void clearerr(FILE *);
int fclose(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE * restrict, fpos_t *);
char *fgets(char * restrict, int, FILE *);

FILE *fopen(const char * restrict __filename, const char * restrict __mode) __asm("_" "fopen" );

int fprintf(FILE * restrict, const char * restrict, ...) __attribute__((__format__ (__printf__, 2, 3)));
int fputc(int, FILE *);
int fputs(const char * restrict, FILE * restrict) __asm("_" "fputs" );
size_t fread(void * restrict __ptr, size_t __size, size_t __nitems, FILE * restrict __stream);
FILE *freopen(const char * restrict, const char * restrict,
                 FILE * restrict) __asm("_" "freopen" );
int fscanf(FILE * restrict, const char * restrict, ...) __attribute__((__format__ (__scanf__, 2, 3)));
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
size_t fwrite(const void * restrict __ptr, size_t __size, size_t __nitems, FILE * restrict __stream) __asm("_" "fwrite" );
int getc(FILE *);
int getchar(void);

...

extern int __vsnprintf_chk (char * restrict, size_t, int, size_t,
       const char * restrict, va_list);
# 408 "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/stdio.h" 2 3 4
# 2 "app.c" 2

int main(void) {
  printf("hello world: %d\n");
  return 0;
}
```
大概就是这么个意义，把所有依赖的.h内容全部merge到一个文件。

stdio.h太复杂，我们改为自己的header试试，
app.c
```
#include "calc.h"

int main(void) {
  int result = add(1, 2);
  return 0;
}
```

calc.h
```
#ifndef CALC_H
#define CALC_H

int add(int a, int b);

#endif
```

生成的app.i很干净
```
# 1 "app.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 362 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "app.c" 2
# 1 "./calc.h" 1



int add(int a, int b);
# 2 "app.c" 2

int main(void) {
  int result = add(1, 2);
  return 0;
}
```

这个时候还不需要calc.c

我们来做个实验，include是不是只能.h文件，能不能多次include
hello.txt
```
hello world
```

app.c
```
#include "hello.txt"
#include "hello.txt"
#include "hello.txt"

int main(void) {
  return 0;
}
```

生成app.i如下
```
# 1 "app.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 361 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "app.c" 2
# 1 "./hello.txt" 1
hello world
# 2 "app.c" 2
# 1 "./hello.txt" 1
hello world
# 3 "app.c" 2
# 1 "./hello.txt" 1
hello world
# 4 "app.c" 2

int main(void) {
  int result = add(1, 2);
  return 0;
}
```

出乎意料吧，显然重复不太好，
修改hello.txt
```
#ifndef HELLO
#define HELLO

hello world

#endif
```
这样就把重复include的问题解决了，加一个变量判断而已，
```
# 1 "app.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 361 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "app.c" 2
# 1 "./hello.txt" 1


hello world

# 2 "app.c" 2


int main(void) {
  int result = add(1, 2);
  return 0;
}
```

小结一下,
- 预处理过程主要是针对一个文件，解析其中以‘#’开始的预编译指令，比如#include, #define等
- 递归处理所有include, 复制文件内容
- 处理所有#define和条件指令 #if, #ifdef #ifndef等
- 删除所有注释
- 添加行号和文件名标识，比如#2 "app.c" 2,辅助后续的错误和警告信息生成
- 保留所有#pragma编译器指令，后续编译阶段需要用到，目前不能删
- 展开所有宏 macro

所以当我们无法判断宏定义的时候，可以直接查看cpp生成的.i文件。

根据以上的
```
#include <stdio.h>
#include "calc.c"

int main(void) {
  printf("1+1=%d", add(1, 1));
  return 0;
}
```
其实这样gcc app.c也是ok，可以正常运行的，你了解这个原理就行。
你完全可以理解为,
```
#include <stdio.h>

int add(int a, int b) {
  return a + b;
}

int main(void) {
  printf("1+1=%d", add(1, 1));
  return 0;
}
```
而引用.h则可以理解为
```
#include <stdio.h>

int add(int, int);

int main(void) {
  printf("1+1=%d", add(1, 1));
  return 0;
}

int add(int a, int b) {
  return a + b;
}
```

其实你想明白就知道原来多文件在这个时候就是针对一个文件，因为其他文件都被include进来了，当然后面我们来解决申明和实现的'对应'。

Compilation (编译)
------------------
$gcc -S app.i -o app.s
等同于
$ccl app.i