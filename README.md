# DIY-crypto
<h6 align="right">创新创业实践项目（Innovation and Entrepreneurship Practice Project）</h6>

### 项目情况

小组成员：鲁逸夫 （个人完成，无组队）

提交方式：通过 `git`  提交代码，`git` 账号名称：`Bruce_Lu`

### 项目清单

- SM3 专题
  - Implement the naïve birthday attack of reduced SM3
  - Implement the Rho method of reduced SM3
  - Implement length extension attack for SM3, SHA256, etc.
  - Do your best to optimize SM3 implementation (software)
  - Implement Merkle Tree following RFC6962
  - Try to Implement .....
- SM4 专题
- SM2 专题
- Bitcoin 专题



### 项目说明

#### SM3 专题

##### 1. Implement the naïve birthday attack of reduced SM3

- **项目简介：** 针对约化后的 SM3（保留输出高位bit）实现朴素生日攻击来寻找碰撞，找到越多 bit 数的碰撞越好。此项目希望在动手实现中加深对课本中生日攻击时间、空间复杂度 $O(2^{n/2})$ 的认识，通过程序运行时的瓶颈体会时间和空间的开销。

- **代码说明：**
  
  - 涉及文件：`DIY-crypto/SM3/sm3_naive_birthday_attack.cpp` 
  - 实现思路：不断产生随机消息和与之对应的哈希值，为了更加高效的查找每次随机生成的消息是否与之前的消息产生碰撞，使用 C++ 标准模板库（STL）中 unordered_map 存储 < 约化后哈希值，消息 > 对，每次 $O(1)$ 查找键值，判断是否存在碰撞。从而在理论上时间和空间复杂度为 $O(2^{n/2})$。更具体的实现细节在代码注释中给出说明。
  
- **运行指导：** 在配置好 openssl 静态/动态库条件下，直接编译运行即可。

- **运行结果：**

  [<img src="https://s1.ax1x.com/2022/07/27/vpZNl9.png" alt="naïve birthday attack of reduced SM3" style="zoom: 67%;" />](https://imgtu.com/i/vpZNl9)

  上图展示的是运行结果，使用21s的时间找到了SM3 48 bit 的碰撞，将消息分别存入文件中，通过命令行计算摘要值验证结果：

  内存限制！

- **参考引用：** 此项目没有参考他人代码，按照对生日攻击的理解独立实现。



##### 2. Implement the Rho method of reduced SM3

- **项目简介：**



- **项目简介：**

- **代码说明：**
- **运行指导：**

- **运行结果：**





- **参考引用：** Rho方法的原理参考 *Introduction of modern cryptography* 一书，代码实现上没有参考他人代码，按照自己的理解独立实现。







[<img src="https://s1.ax1x.com/2022/07/27/vpZh0P.png" alt="Rho method of reduced SM3.png" style="zoom:67%;" />](https://imgtu.com/i/vpZh0P)



##### 3、Implement length extension attack for SM3, SHA256, etc.

- **项目简介：**

- **代码说明：**
- **运行指导：**

- **运行结果：**

- **参考引用：**



##### 4、Do your best to optimize SM3 implementation (software)

- **项目简介：**

- **代码说明：**
- **运行指导：**

- **运行结果：**

- **参考引用：**



##### 5、Implement Merkle Tree following RFC6962

- **项目简介：**

- **代码说明：**
- **运行指导：**

- **运行结果：**

- **参考引用：**



【说明】因为刚开始不会用 `git`，一直通过 `github` 网页端提交更新项目，后来在提交前才学会 `git` 的使用方法，但是由于本地仓库和远程仓库都已经存在文件，建立连接时一直出问题（也有对 git 命令不熟悉的缘故 orz），所以全部文件的提交时间都比较近，希望显示的提交时间能够不影响成绩。
