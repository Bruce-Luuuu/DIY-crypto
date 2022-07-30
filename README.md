# DIY-crypto
<h6 align="right">创新创业实践项目（Innovation and Entrepreneurship Practice Project）</h6>

### 项目情况

小组成员：鲁逸夫 201900460002（个人完成，无组队）

git 账户名称：`Bruce_Lu`

个人贡献：完成下面清单中勾选的 7 个项目

### 项目清单

#### SM3 专题

- [x] Implement the naïve birthday attack of reduced SM3 

- [x] Implement the Rho method of reduced SM3

- [x] Implement length extension attack for SM3, SHA256, etc.

- [ ] Do your best to optimize SM3 implementation (software)

- [x] Implement Merkle Tree following RFC6962

#### SM4 专题

- [x] Do your best to optimize SM4 implementation (software)

#### SM2 专题

- [x] Report on the application of this deduce technique in Ethereum with ECDSA

- [ ] Implement sm2 with RFC6979

- [ ] Verify the above pitfalls with proof-of-concept code

- [ ] Implement the above ECMH scheme

- [ ] Implement a PGP scheme with SM2

- [ ] Implement sm2 2P sign with real network communication

- [ ] Implement sm2 2P decrypt with real network communication

- [ ] PoC impl of the scheme, or do implement analysis by Google

#### Bitcoin 专题

- [ ] Send a tx on Bitcoin testnet, and parse the tx data down to every bit, better write script yourself
- [ ] Forge a signature to pretend that you are Satoshi

#### Ethernet 专题

- [x] Research report on MPT

#### Real world cryptanalyses 专题

- [ ] Find a key with hash value `sdu_cst_20220610` under a message composed of *your name* followed by *your student ID*. For example, “*San Zhan 202000460001*”.
- [ ] Find a 64-byte message under some k fulfilling that their hash value is symmetrical

#### Real world zk-SNARKs 专题

- [ ] Project Idea：

  - Write a circuit to prove that your CET6 grade is larger than 425.
    a. Your gradeinfo is like(cn_id, grade, year, sig_by_moe). These grades are published as commitments onchain by MoE.
    b. When you got an interview from an employer,you can prove to them that you have passed the exam without letting them know the exact grade.

  - The commitment scheme used by MoE is SHA256-bases.
    a. commit=SHA256(cn_id，grade，sig_by_moe，r)



### 项目说明

- 代码主要使用 C、C++、python 这 3 种语言：由于 python 的依赖库便于安装且代码使用的库较少，因此不难正常运行代码；而其他大部分项目用到了 C 的 openssl 库，在本地使用 Visual Studio 配置时，我直接链接静态库编译生成可执行文件，为确保验收项目时不存在第三方库缺失、路径依赖等问题，我将 openssl 静态库上传至仓库中（`DIY-crypto/openssl_lib`）并在每个项目通过相对路径链接库，因此只要有VS开发环境，应当可以正常编译运行各个代码。
- 各项目具体的说明分别在每个项目对应文件夹下的 `README.md` 文件中，其中包括项目简介、代码说明、运行指导、运行结果和参考引用等内容。
- 完成的项目为上面清单中勾选的 7 个项目，其余项目未完成，所有完成项目均可按要求正常运行得到结果（无问题）。

下面给出完成的每个项目对应的文件位置

#### 1. Implement the naïve birthday attack of reduced SM3

相关文件位置：`DIY-crypto/SM3/Sm3_birthday_attack` 

#### 2. Implement the Rho method of reduced SM3

相关文件位置：`DIY-crypto/SM3/Sm3_improved_birthday_attack` 

#### 3. Implement length extension attack for SM3, SHA256, etc.

相关文件位置：`DIY-crypto/SM3/Sm3_length_extension_attack` 

#### 4. Implement Merkle Tree following RFC6962

相关文件位置：`DIY-crypto/SM3/Merkle_tree` 

#### 5. Do your best to optimize SM4 implementation (software)

相关文件位置：`DIY-crypto/SM4/SM4_optimized` 

#### 6. Report on the application of this deduce technique in Ethereum with ECDSA

相关文件位置：`DIY-crypto/SM2/report_on_deduce_technique`

#### 7. Research report on MPT

相关文件位置：`DIY-crypto/ETHERNET/MPT_research`

