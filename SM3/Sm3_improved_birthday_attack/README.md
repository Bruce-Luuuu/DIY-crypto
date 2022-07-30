## Implement the Rho method of reduced SM3

### 项目简介

此项目针对约化后的 SM3（保留输出高位bit）实现 Rho 方法来寻找碰撞，找到越多 bit 数的碰撞越好。Rho 方法是在原本朴素生日攻击的基础上，对空间复杂度进行改进，使得原本 $O(2^{n/2})$ 的空间开销降为 $O(1)$ 。Rho 方法寻找碰撞的思想在密码学中应用广泛，不仅在哈希函数寻找碰撞中使用，还可用于求解离散对数问题等。

### 代码说明

- 代码文件：`sm3_rho_method_attack.cpp`

- 实现思路：Rho 方法原理参考 *Introduction of modern cryptography*  书所述，书中给出的伪代码为：

  ​                            <img src="https://s1.ax1x.com/2022/07/27/vpGyhn.png" alt="vpGyhn.png" width="470px" />

  代码的整体逻辑框架基本与伪代码相同，更具体的实现细节在代码注释中给出说明。不仅如此，书中给出了命题 “当前 q 个元素中存在重复时，该算法至多在第一个循环进行 q 次迭代就可以找到一个重复。”的严格证明，从而保证算法时间复杂度为$O(2^{n/2})$；又因为算法只需要存储 2 个哈希值，因此空间复杂度为 $O(1)$。

### 运行指导

在存在 Visual Studio 软件开发环境条件下，双击 `.sln` 文件加载整个项目文件，然后直接编译运行即可（推荐在 Release 模式下编译，如果平台是64位则选择 x64，否则 x86）。

### 运行结果

由于 Rho 方法不像朴素的生日攻击受内存的限制，因此程序运行的时间越长，理论上可以暴力找到的碰撞 bit 数越多，这里分别成功地找到了 SM3 高位 **56 bit** 和 **64bit** 的碰撞：

<img src="https://s1.ax1x.com/2022/07/27/vpZh0P.png" alt="Rho method of reduced SM3.png" width="700px" />

由上图运行结果知，Rho 方法在 855s（约14分钟）找到了 SM3 高 **56 bit** 的碰撞。将消息分别存入文件 `a56.txt` 和 `b56.txt` 中，通过命令行计算摘要值验证结果：

<img src="https://s1.ax1x.com/2022/07/30/vFkoH1.png" alt="vFkoH1.png" width="700px" />

因为 Rho 方法需要对消息哈希，所以无法保证最后找的的碰撞消息对有意义，故这里两个文件中的内容与朴素生日攻击中展示的不同，在从字节值按记事本默认转码过程中出现乱码现象。

---

<img src="https://s1.ax1x.com/2022/07/30/vFAk8S.png" alt="vFAk8S.png"  width="700px" />

又运行程序，Rho 方法在 7677s（约2小时）找到了 SM3 高 **64 bit** 的碰撞。将消息分别存入文件 `a64.txt` 和 `b64.txt` 中，通过命令行计算摘要值验证结果：

<img src="https://s1.ax1x.com/2022/07/30/vFAhPf.png" alt="vFAhPf.png" width="700px" />

### 参考引用

Rho方法的原理参考 *Introduction of modern cryptography* 一书，代码实现上没有参考他人代码，按照自己的理解独立实现。