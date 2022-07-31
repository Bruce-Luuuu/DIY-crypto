## Do your best to optimize SM4 implementation (software)

### 项目简介

尝试使用各种方法优化 SM4 的软件实现效率，课上提供的思路有：1. 使用循环展开加速	2. 使用宏定义避免函数调用的开销	3. 使用 SIMD 指令集优化，具体有同时对多个独立的消息流进行哈希计算，或者对同一个消息流中多个中间变量同时进行运算 2 种不同的方法。

### 代码说明

#### 代码文件

`main.cpp`：主函数用于测试加密不同。

`SM4.h`：包括 SM4 加解密函数、轮密钥生成函数和密钥销毁函数的声明。

`SM4.cpp `：包括 SM4 各函数的定义，以及S盒、FK、CK等系统参数的定义。

#### 实现思路

1、查表优化



2、SIMD

主要用到的指令有：





### 运行指导

在存在 Visual Studio 软件开发环境条件下，双击 `.sln` 文件加载整个项目文件，然后直接编译运行即可（推荐在 Release 模式下编译，如果平台是64位则选择 x64，否则 x86）。

### 运行结果



效率的两个指标：每秒能够加密内存中多少数据（效率高低不光与实现，还与CPU主频有关系）；加密一个字节Bytes需要多少时钟周期（受CPU架构影响小）



<img src="https://s1.ax1x.com/2022/07/31/vFJQmt.png" alt="vFJQmt.png" width="700px" />



### 参考引用

[1] [Intel SIMD 指令集官方文档](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)

[2] [郎欢, 张蕾, 吴文玲. SM4的快速软件实现技术[J]. 中国科学院大学学报](http://html.rhhz.net/ZGKXYDXXB/20180205.htm)

[3] [博客：SM4 SIMD 指令集优化(intel)](https://www.cnblogs.com/kentle/p/15562530.html)

[4] 《商用密码算法原理与C语言实现》

