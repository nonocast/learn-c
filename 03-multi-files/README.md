gcc从app.c到a.out分为4个步骤:
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
编译器就是将高级语言翻译成CPU相关的汇编语言的过程。

`$gcc -S app.i -o app.s`
等同于
`$cc app.i`

app.s
```
	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16G
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movl	$0, -4(%rbp)
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
```

你可以通过`gcc -S app.c`直接完成propressing和compilation得到.s文件。

Assembly (汇编)
-----------
`gcc -c app.s -o app.o`
等同于
`as app.s -o app.o`

或者通过-c直接跳步
`gcc -c app.c -o app.o`

这里的.o文件称为Object File(目标文件),Object File是不可以执行的，
这里先不展开编译和汇编的内容，我们接着往下开Linking部分

Linking (链接)
--------------
首先linking用到的是ld，称之为linker，linker作用就是link各种object files, libraries后生成可执行文件(output file)，ld可以生成一个最终的linked image, 可执行文件或者库文件。

最简单的app.c
```
int main(void) {
  return 0;
}
```

在mac上linker只需要
```
gcc -c app.c -o app.o
ld app.o -lSystem
```

这里的-lSystem中-l表示增加library，而System表示/usr/lib/libSystem.dylib,如果-lssl，则表示/usr/lib/libssl.dylib
[ld(1) [osx man page]](https://www.unix.com/man-page/osx/1/ld/)

多平台情况
--------
我们知道,windows的exe可以从一个机器复制到另外一台机器，但是linux每次都要make configure, make然后install，这是为什么呢?
个人理解:
1. windows可执行程序采用PE格式，而linux采用ELF，所以可执行文件肯定不能跨系统运行
2. windows只支持x86体系，所以汇编包括链接是一致的，所以可以跨设备运行
3. linux不仅支持x86,更是可以支持mips, arm等众多cpu, 所以跨cpu的会得到不同的汇编结果，所以需要重新汇编
4. 在当前平台编译别的平台运行的程序称为交叉编译，比如在windows或mac上编译android系统的程序就是一种典型的交叉编译，又比如编译单片机程序
5. 同样一个hello world的c文件在不同的系统上有完全不同的编译链路，windows, mac, linux又或者嵌入式系统


Mac上交叉编译Android的hello world
--------------------------------
启动android模拟器,
`emulator -list-avds`
`emulator -avd xxx`

这里就需要用到ndk作为交叉编译的工具链，
补充一下交叉工具命名一般规则: arch-[vendor]-kernel-system-toolname
- arch，指的是CPU架构, 一般包括如下几种架构: arm, mips, powerpc, x86, x86_64。
- verdor, 一般指的是生产厂商, 如果没有生产厂商可以为空。
- kernel, 指的目标环境使用的 kernel，以android为例，它使用的是 linux内核，所以在这部分会填写为linux。
- system, 指的是那个系统, 如androideabi, android
- toolname: 指的是 gcc, ld, ar等。

android对应的就是arm-linux-andirod-gcc

常用的环境变量:
- PREFIX: 指明交叉编译后输出的目录。
- ARCH: 指明交叉编译后输出的CPU架构。
- CROSS-PREFIX：指明交叉编译前辍 arch-vender-kernel-system
- SYSROOT: 指明交叉编译目标机器的头文件和库文件目录
- TOOLCHAIN: 指明交叉编译工具链的位置。
- PLATFROM： 指明交叉编译时使用的是哪个版本的的头文件和库文件。它是 SYSROOT的一部分。
- ANDROID_NDK: 指明 Android NDK 所在目录。

android ndk官方定义宿主系统如下:
- macOS: darwin-x86_64
- Linux: linux-x86_64
- 32-bit Windows: windows
- 64-bit Windows: windows-x86_64

目标系统tag如下:
- armeabi-v7a: armv7a-linux-androideabi
- arm64-v8a: aarch64-linux-android
- x86: i686-linux-android
- x86-64: x86_64-linux-android

使用 NDK 编译代码主要有三种方法：
- 基于 Make 的 ndk-build。
- CMake。
- 独立工具链，用于与其他编译系统集成，或与基于 configure 的项目搭配使用。

通过下面的片段我们可以看到整个工具链的构成
```
$ export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/$HOST_TAG
$ export AR=$TOOLCHAIN/bin/aarch64-linux-android-ar
$ export AS=$TOOLCHAIN/bin/aarch64-linux-android-as
$ export CC=$TOOLCHAIN/bin/aarch64-linux-android21-clang
$ export CXX=$TOOLCHAIN/bin/aarch64-linux-android21-clang++
$ export LD=$TOOLCHAIN/bin/aarch64-linux-android-ld
$ export RANLIB=$TOOLCHAIN/bin/aarch64-linux-android-ranlib
$ export STRIP=$TOOLCHAIN/bin/aarch64-linux-android-strip
```

google采用了clang取代gcc/g++，其实大同小异。

什么是ABI?
不同的 Android 手机使用不同的 CPU，而不同的 CPU 支持不同的指令集。CPU 与指令集的每种组合都有专属的应用二进制接口，即 ABI。ABI 可以非常精确地定义应用的机器代码在运行时如何与系统交互。您必须为应用要使用的每个 CPU 架构指定 ABI。
armeabi-v7a, arm64-v8a, x86, x86-64就是响应的abi

下载NDK
======
1. 直接从官方[NDK Downloads](https://developer.android.com/ndk/downloads/index.html)下载
2. 通过Android Studio/SDK Manager，下载后在/Users/nonocast/Library/Android/sdk/ndk/20.0.5594570

首先确定目标设备的abi,因为我是在mac上的emulator，所以肯定是x86的，
```
$adb shell getprop ro.product.cpu.abilist
x86
```
Mipad
```
$ adb shell getprop ro.product.cpu.abi    
armeabi-v7a
```

生成对应arch的toolchain,
```
python make_standalone_toolchain.py --arch arm --install-dir ~/toolchain/arm
python make_standalone_toolchain.py --arch arm64 --install-dir ~/toolchain/arm64
python make_standalone_toolchain.py --arch x86 --install-dir ~/toolchain/x86
```

```
// mipad
/Users/nonocast/toolchain/arm/bin/clang app.c -o app
✗ file app
app: ELF 32-bit LSB shared object, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /system/bin/linker, not stripped

// emulator
/Users/nonocast/toolchain/x86/bin/clang app.c -o app
➜  src git:(master) ✗ file app
app: ELF 32-bit LSB shared object, Intel 80386, version 1 (SYSV), dynamically linked, interpreter /system/bin/linker, not stripped
```

[Cross Compiling C/C++ for Android - Nick Desaulniers](http://nickdesaulniers.github.io/blog/2016/07/01/android-cli/)
[搞机：AS自带模拟器AVD Root 和 Xposed安装 - 掘金](https://juejin.im/post/5cd2839de51d453a6c23b080)

```
$ adb push app /data/local/tmp
$ adb shell /data/local/tmp/app
hello world
```


继续
----
app.c
```
int add(int, int);

int main(void) {
  add(1, 2);
  return 0;
}
```

我们接着来看add这个方法,
- prepressing: `gcc -E app.c -o app.i`
```
# 1 "app.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 362 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "app.c" 2
int add(int, int);

int main(void) {
  add(1, 2);
  return 0;
}
```
- compilation: `$gcc -S app.i -o app.s`
```
	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	movl	$1, %edi
	movl	$2, %esi
	callq	_add
	xorl	%esi, %esi
	movl	%eax, -8(%rbp)          ## 4-byte Spill
	movl	%esi, %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols

```

对比有add方法定义的app.c生成的asm
app.c
```
int add(int a, int b) {
  return a + b;
}

int main(void) {
  add(1, 2);
  return 0;
}
```

app.s
```
	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15
	.globl	_add                    ## -- Begin function add
	.p2align	4, 0x90
_add:                                   ## @add
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %esi
	addl	-8(%rbp), %esi
	movl	%esi, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	movl	$1, %edi
	movl	$2, %esi
	callq	_add
	xorl	%esi, %esi
	movl	%eax, -8(%rbp)          ## 4-byte Spill
	movl	%esi, %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
```

对比发现缺少_add的定义，没法完成callq _add

最终导致link error
```
gcc -c app.s -o app.o
➜  src git:(master) ✗ ld app.o -lSystem
Undefined symbols for architecture x86_64:
  "_add", referenced from:
      _main in app.o
ld: symbol(s) not found for architecture x86_64
```
换句话说，_add是在link阶段完成申明和调用的对接。


缺什么补什么，`gcc -S calc.c -o calc.s`
calc.s
```
	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15
	.globl	_add                    ## -- Begin function add
	.p2align	4, 0x90
_add:                                   ## @add
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %esi
	addl	-8(%rbp), %esi
	movl	%esi, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
```
然后生成app.o
`gcc -c calc.s -o calc.o`

最后link的时候合并，
`ld app.o calc.o -lSystem -o app`

搞定。
回过头来，没有.h一样可以搞定link
所以，.h只不过是提取公因式，没有什么奥秘，无形中形成一个申明定义而已。

app.c
```
#include <stdio.h>
#include "calc.h"

int main(void) {
  printf("1+2: %d\n", add(1, 2));
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

calc.c
```
#include "calc.h"

int add(int a, int b) {
  return a + b;
}
```

`gcc app.c calc.c -o app`

分解步骤
- 针对每个.c编译汇编生成object file(.o)
- 将object file link生成可执行文件

lib是什么？
-------------------
lib就是.o的container
通过ar来完成打包

```
$ ar -crv libcalc.a calc.o
$ file libcalc.a
libcalc.a: current ar archive random library
```
linux下lib一般的命名规则:libxxx.a

可以通过ar -t查看，
```
$ar -t libcalc.a
__.SYMDEF SORTED
calc.o
```
通过nm查看object file中的symbol
```
$ nm libcalc.a

libcalc.a(calc.o):
0000000000000000 T _add
```

client如何调用呢?
`ld app.o -lSystem -L. -lcalc -o app`app.c
```
回忆一下之前的link
`ld app.o calc.o -lSystem -o app`

-L告诉ld link directory，然后-l表示lib name
```
-lx
This option tells the linker to search for libx.dylib or libx.a in the library search path.  If string x is of the form y.o, then that file is searched for in the same places, but without prepending `lib' or appending `.a' or `.dylib' to the filename.

-Ldir
Add dir to the list of directories in which to search for libraries.  Directories specified with -L are searched in the order they appear on the command line and before the default search path. In Xcode4 and later, there can be a space between the -L and directory.
```

这样就通过lib完成了模块分离。
静态的好处是容易理解，但问题是lib升级就需要重新编译app，不能自行完成升级，不能被多个app共享。

补充一句gcc link方式,
`gcc app.c -L. -lcalc -o app`

[All about Static Libraries in C - megha mohan - Medium](https://medium.com/@meghamohan/all-about-static-libraries-in-c-cea57990c495)

下面来看动态库，linux下是so, windows下是dll, mac下则是dylib
`gcc -shared calc.o -o libcalc.so`
`gcc app.c -L. -lcalc -o app`

当同时存在libcalc.a和libcalc.so时，gcc会优先采用so方式，
同样的语句link app这时候会动态link到libcalc.so, 验证一下，
```
$ file app
app: Mach-O 64-bit executable x86_64
$ du -h app
16K    app
$ otool -L app
app:
        libcalc.so (compatibility version 0.0.0, current version 0.0.0)
        /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1279.0.0)
```

对比一下之前libcalc.a的链接，
```
$ otool -L app
app:
        /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1279.0.0)
```

mac的otool对应linux下的ldd.

此时如果移除so则运行会报错,
```
./app
dyld: Library not loaded: libcalc.so
  Referenced from: /Users/nonocast/Developer/projects/learn-c/03-multi-files/src/./app
  Reason: image not found
[1]    28999 abort      ./app
```
将libcalc.so ln到/usr/local/lib

```
ln -s ~/Developer/projects/learn-c/03-multi-files/src/libcalcx.so /usr/local/lib/libcalc.so
./app
1+2=3
```
那么搜索so的默认路径是什么呢?
默认值: $(HOME)/lib:/usr/local/lib:/lib:/usr/lib

确实如此，放到~/lib下，也能work。

[Linux Tutorial - Static, Shared Dynamic and Loadable Linux Libraries](http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html)
[OS X 下动态库的引用 - 刘宝成 - 博客园](https://www.cnblogs.com/liubaocheng999/p/4285256.html)

#EOF