Chapter 7 链接
==============

### 编译器驱动程序
&emsp;&emsp;其实平时理解的**编译器**是“编译器驱动程序”。其中包括了预处理器、编译器、汇编器和连接器。

所以一个程序从源代码诞生经历了如下过程：<br>
1. __预处理器__将源代码如`main.c`翻译成一个ASCII码的中间文件`main.i`<br>
2. __编译器__开始运行，将`main.i`翻译成ASCII码的汇编文件`main.s`<br>
3. __汇编器__将`main.s`翻译成一个**可重定位目标文件** `main.o`<br>
4. __连接器__开始工作，将`main.o`与其他可重定位目标文件以及一些必要的系统目标文件组合起来形成**可执行目标文件**。<br>

### 静态链接
&emsp;&emsp;链接器的工作主要有**符号解析**与**重定位**。理解 **符号解析**特别有助于理解如何处理**非静态全局变量**，`static`变量等等。

#### 符号解析
>每个可重定位目标模块*m*都有一个符号表，它包含*m*所定义和引用的符号的信息。在连接器的上下文中，有三种不同的符号：<br>

* 由*m*定义并能被其他模块引用的**全局符号**。 对应的是非`static`函数与非`static`变量
* 由其他模块定义被模块*m*引用的**全局符号**。这些符号称为**外部符号**。对应定义在其他模块中的函数与变量。
* 只被模块m定义和引用的**本地符号**。也就是本地`static`变量与函数。

>本地连接器符号与本地程序变量是不同的。符号表`.symtab`中不包含本地非静态程序变量的任何符号。**本地程序变量就是局部变量**。

>连接器解析符号引用的方法是将每个引用与它输入的可重定位目标文件的符号表中的一个确定符号定义起来。

&emsp;&emsp;如果是引用了 定义在相同模块中本地符号的引用，符号解析就很简单明了。
规则就是编译器只允许每个模块中本地符号只有**一个**定义。

&emsp;&emsp;对全局符号的引用解析则是，如果**编译器**遇到一个符号（变量或函数）没有定义在当前模块，它就假设这个符号在其它模块里面定义，然后生成一个**链接器符号表条目**，剩下的就交给链接器来处理了。

&emsp;&emsp;接下来的就是链接器会在每一个输入模块里面找这个被引用的符号，如果都找不到，就输出一个链接错误，可能是`unresolved external symbol`或`undefined reference to xxx`。

但是还有可能出现多个模块定义了相同的全局符号，对这种多重定义符号的解析遵循一定的规则。

##### 解析多重定义符号
全局符号分为`strong`和`weak`两种。已初始化的全局变量与函数是`strong`的，而未初始化的全局变量是`weak`的。

Unix链接器根据如下规则处理多重定义符号

* 不允许多个强符号
* 如果有一个强符号多个弱符号，则选择强符号
* 如果有多个弱符号，就随机选一个 

这些规则看起来很容易，但是还是有一些有意思的问题。比如说有两个源文件`foo.c` `bar.c` 。<br>

	/* foo.c */
	#include <stdio.h>
	void f(void);
	
	int x = 12345;
	int y = 12345;

	int main()
	{
		f();
		printf("x = 0x%x y = 0x%x \n", x, y);
		return 0;
	}

在`foo.c`中定义了int型的x,y。

	/* bar.c */
	double x;
	
	void f()
	{
		x = -0.0;
	}

在`bar.c`中仅声明了double型的x,所以是`weak`的。这在链接的过程中是会成功的，链接器选择了int x。但是调用f()的时候，问题来了，f()里面对x进行赋值，这不属于链接器管的范畴，这是最早编译器处理的。编译器看到了有x的引用，就在模块里找，发现找到了double x，那就OK了。所以在函数`f()`里面x是`double`类型的。但是这个x的地址也就是`foo.c`里面int x的地址，而32位机器上double是8位的，int是4位的，这么赋值就会造成`foo.c`里面的x,y都会改变了。 **有意思吧**。




