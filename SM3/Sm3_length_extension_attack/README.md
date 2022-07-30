## Implement length extension attack for SM3, SHA256, etc.

### 项目简介

由于 SM3、SHA256 等都是 Merkle-Damgard 结构的哈希函数，而长度扩展攻击是针对 MD 结构的常见攻击，此项目就是要实现对 SM3 的长度扩展攻击。

对于一般 MD 结构的哈希函数，长度扩展攻击的原理为：给定杂凑值 $H(M)$ 和消息的长度 $|M|$，无需知道消息 $M$ ，令 $z= 0^d~\|~|M|~ \|~x$，其中 $0^d~\|~|M|$ 为原始消息 $M$ 的填充信息（一般为消息长度）， $x$ 为敌手任意选择的消息，则根据 $H(M)$ 可计算 $h^′$,满足 $H(M||z)=h'$。具体过程如下图所示：

<img src="https://s1.ax1x.com/2022/07/30/vFZCU1.png" alt="vFZCU1.png" width="490px" />

<img src="https://s1.ax1x.com/2022/07/30/vFZP4x.png" alt="vFZP4x.png" width="530px" />

### 代码说明

- 代码文件：`main.cpp`	`SM3.cpp `	`SM3.hpp`
- 实现思路：这里主要针对 SM3 长度扩展攻击的实现而介绍，更具体的代码实现细节见代码注释。

### 运行指导

在存在 Visual Studio 软件开发环境条件下，双击 `.sln` 文件加载整个项目文件，然后直接编译运行即可（推荐在 Release 模式下编译，如果平台是64位则选择 x64，否则 x86）。

### 运行结果



<img src="https://s1.ax1x.com/2022/07/30/vFZmbd.png" alt="vFZmbd.png" width="1000px" />



### 参考引用

此项目中 SM3 代码参考《商用密码 算法原理与C语言实现》一书补充改编而成，长度扩展攻击无参考，按照自己的理解实现。