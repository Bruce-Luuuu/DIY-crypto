## Do your best to optimize SM4 implementation (software)

### 项目简介

尝试使用各种方法优化 SM4 的软件实现效率，课上提供的思路有：1. 使用循环展开加速	2. 使用宏定义避免函数调用的开销	3. 使用 SIMD 指令集优化，具体有同时对多个独立的消息流进行哈希计算，或者对同一个消息流中多个中间变量同时进行运算 2 种不同的方法。

本项目主要的优化效果是由查表优化和 SIMD 并行技术带来的，其中因为 SM4 的设计，使得在密码算法内部并行处理消息比较困难，因此这里采用多个消息并行处理，即粗粒度下运用 SIMD 技术进行加速。

### 代码说明

#### 代码文件

`main.cpp`：主函数用于测试加密不同。

`SM4.h`：包括 SM4 加解密函数、轮密钥生成函数和密钥销毁函数的声明。

`SM4.cpp `：包括 SM4 各函数的定义，以及S盒、FK、CK等系统参数的定义。

#### 实现思路

**1、查表优化**

查表是密码算法软件实现的最基本方法，该方法最早是 Daemen 和 Rijmen 在 32 bit 处理器上实现 Rijndael 时提出的。此方法的核心思想是将密码算法轮函数中尽可能多的变换操作制成表格，运行时直接读取值以加速运行效率。

对于 SM4，其每一轮迭代流程如下图所示：

<img src="https://s1.ax1x.com/2022/07/31/vFa2es.png" alt="vFa2es.png" width="450px;" />

轮函数 $F$ 是由非线性变换 $\tau$ 和线性变换 $L$ 组成，即 $F(x)=L(\tau(X))$，输入输出都是 32 比特字。

其中 $\tau$ 又是由 4 个并行的 8 进 8 出 $S$ 盒组成，将 32 比特字分为 4 个字节并行过 $S$ 盒，可以表示为 $\tau(X)=(S(x_1),S(x_2),S(x_3),S(x_4))$ ；线性变换 $L$ 可以表示为 $L(B)=B \oplus (B \mathtt{<<<} 2) \oplus (B \mathtt{<<<} 10) \oplus (B \mathtt{<<<} 18) \oplus (B \mathtt{<<<} 24)  $.

为了提升效率，可以将 $S$ 盒与线性变换 $L$ 合并，通过 

$F(x)=L(S(x_0), S(x_1), S(x_2), S(x_3))
=L(S(x_0)\mathtt{<<<}24)⊕L(S(x_1)\mathtt{<<<}16)⊕L(S(x_2)\mathtt{<<<}8)⊕L(S(x_3))$

可以定义 4 个 8 进 32 出的 T-table，满足：

$\mathtt{T-table_0}(x)=L(S(x)\mathtt{<<<}24)$

$\mathtt{T-table_1}(x)=L(S(x)\mathtt{<<<}16)$

$\mathtt{T-table_2}(x)=L(S(x)\mathtt{<<<}8)$

$\mathtt{T-table_3}(x)=L(S(x))$

这样每次可以通过查表节省循环移位操作，此优化方案的基本思想是空间换时间。

**2、SIMD**

使用 Intel 架构的 SIMD 指令，用到的主要指令有：

|        函数名称        |                           操作说明                           |
| :--------------------: | :----------------------------------------------------------: |
| _mm256_i32gather_epi32 |                        32bit 并行查表                        |
|   _mm256_loadu_si256   |                       256bit 数据加载                        |
|  _mm256_storeu_si256   |                       256bit 数据存储                        |
|   _mm256_slli_epi32    |                        32bit 循环左移                        |
|   _mm256_srli_epi32    |                        32bit 循环右移                        |
|    _mm256_set_epi32    |            32bit 分组数据装载（4个字置成不同值）             |
|   _mm256_set1_epi32    |           32bit 分组数据装载（4个字都置成相同值）            |
|    _mm256_xor_si256    |                         256bit 异或                          |
|  _mm256_shuffle_epi8   |             按字节为单位重排（顺序作为参数传入）             |
| _mm256_unpacklo_epi32  | 将传入的 2 个128 bit 参数 低位比特按 32 位解包并交错存储在结果中返回 |
| _mm256_unpacklo_epi64  | 将传入的 2 个128 bit 参数 低位比特按 64 位解包并交错存储在结果中返回 |
| _mm256_unpackhi_epi32  | 将传入的 2 个128 bit 参数 高位比特按 32 位解包并交错存储在结果中返回 |
| _mm256_unpackhi_epi64  | 将传入的 2 个128 bit 参数 高位比特按 64 位解包并交错存储在结果中返回 |

主体思路仍是按照 SM4 算法正常执行，只不过每次同时处理 8 个消息分组。



### 运行指导

在存在 Visual Studio 软件开发环境条件下，双击 `.sln` 文件加载整个项目文件，然后直接编译运行即可（推荐在 Release 模式下编译，如果平台是64位则选择 x64，否则 x86）。

### 运行结果

衡量加密算法的效率一般有两种指标较常见：一是每秒能够加密内存中多少数据；二是加密一个字节需要多少时钟周期。这里采用第一种方法，在实验中分别统计加密长度为 $16, 64, 256, 1024, 8192, 16384$ 个分块长度的随机消息，在 3s 内一共能加密多少次。测试结果见下图：

<img src="https://s1.ax1x.com/2022/07/31/vF6BYd.png" alt="vF6BYd.png" width="580px;" />

为了对比说明其效率，以 openssl 为参照，在默认 CBC 模式下调用 SM4 进行加密：

<img src="https://s1.ax1x.com/2022/07/31/vFJQmt.png" alt="vFJQmt.png" width="700px" />

可以大致看出 openssl 的效率是 实现的 SM4 效率的 $2\sim 5$ 倍，说明 SIMD + 查表优化的效果还是较为明显的，但相比 openssl 仍存在差距。



### 参考引用

[1] [Intel SIMD 指令集官方文档](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)

[2] [郎欢, 张蕾, 吴文玲. SM4的快速软件实现技术[J]. 中国科学院大学学报](http://html.rhhz.net/ZGKXYDXXB/20180205.htm)

[3] [博客：SM4 SIMD 指令集优化(intel)](https://www.cnblogs.com/kentle/p/15562530.html)

[4] 《商用密码算法原理与C语言实现》

[5] [Crypto++库中sm4的SIMD实现代码](https://github.com/weidai11/cryptopp/blob/master/sm4_simd.cpp)
