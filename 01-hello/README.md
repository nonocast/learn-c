C的历史和标准化进程
* 1967年，Martin Richards为写操作系统和编译器开发了BCPL
* 随后，Ken Thompson改进BCPL，形成了B，取B是因为ALGOL 60中的字母A
* 1970年，Ken在贝尔实验室(如果你对贝尔实验室没有概念的话，可以看一下浪潮之巅，就知道当时有多牛X)用B在DEC PDP-7写了第一个UNIX
* 1973年，Dennis Ritchie为了将UNIX从PDP-7移植到PDP-11上，在B的基础上形成C，并用C重写了UNIX，至此形成C的原型
* 1978年，Dennis Ritchie和Brian Kernighan合写了K&R C
* 1989年，为了解决不兼容问题，ANSI对C进行了标准化，此标准称为C89
* 1999年，针对C89进行了一些更新，此版本称为C99
* 2011年12月8日，ISO正式发布了新的C语言的新标准C11，之前被称为C1X，官方名称为ISO/IEC 9899:2011

从1973年算起，也差不多有40年了，其中主要的4个版本
* Classic C
* C89
* C99
* C11

简单的理解，C语言与其他语言间区别可以用3个形容词:1底层，2简约，3自由。Windows/Linux的核心代码全是由C完成的，足见其底层，在K&R中只有了49页就描述了整个C语言，足见其小巧简约，C没有设定太多的限制，他是基于用户知道在做什么的前提下给出提示，而不会很刀刀的说这不行那不行，你请便，不过后果自负。

```
int main(void) { ... }
```
如果有参数
```
int main(int argc, char *argv[]) { ... }
```

一个完整的C System由3个部分组成，即环境、语言和C标准库。我个人理解是语言本身包括词法、语法和表达式的规则，而C标准库则是由一群.h组成的接口定义，实现则交由个编译器执行实现，因为不同平台的实现方式必然不相同。环境比较复杂，一个由C语言编写的文件需经过预处理、编译、连接最终形成可执行文件，UNIX体系最常见的是gcc(不仅仅是gcc)，而Windows则对应CL和LINK。
UNIX可以参考gcc从语言编译全过程 预处理—->编译—->汇编—–>链接，已经说的很清晰了，这里我们以Windows来具体描述C System，在Windows下C System是随Visual Studio出现的，其中包括了编译器、链接器和标准库接口和实现。

* 编译器
CL.EXE是微软针对Windows提供的C编译器。以VS2010为例，cl.exe在C:\Program Files\Microsoft Visual Studio 10.0\VC\bin\下，将此目录添加到系统Path中，这样在命令行下可以直接调用。
* 连接器
LINK则是微软提供的C连接器，根据MSDN描述，CL会执行在需要的情况下调用LINK做连接，所以我们通常只用到CL
* C标准库接口
位于C:\Program Files\Microsoft Visual Studio 10.0\VC\include，你可以在里面找到stdio.h之类的
* C标准库实现
位于C:\Program Files\Microsoft Visual Studio 10.0\VC\lib，微软最初提供的是libc.lib，而到了VS2005以后推荐使用libcmt.lib，包括多线程的内容，无需再使用single thread的libc.lib，具体参见这里
为了验证其中lib中确实包含了实现，我们可以通过命令行输入lib /list libcmt.lib来查查看其中包含的obj，可以将lib理解为一组obj的包(zip形式)
最后以编译hello.c为例描述整个流程的分解动作：
* 预处理
预处理过程输入.c输出.i
在命令行中运行cl /I”C:\Program Files\Microsoft Visual Studio 10.0\VC\include” /P hello.c
运行后会在hello.c的目录下生成hello.i的预处理后文件，你会惊讶的发现原本只有6行的代码被搞大到68k共5376行，恐怖吧…
可以将include目录设为环境变量(include=C:\Program Files\Microsoft Visual Studio 10.0\VC\include)，cl会自动获取，lib也同样如此。
* 编译
编译的第一个阶段将C语言翻译成汇编语言，输入.i输出.asm
在命令行中运行cl /c /Fa hello.c时会生成一个36行的hello.asm文件，/c表示compile only, no link