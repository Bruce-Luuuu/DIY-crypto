#pragma once

#include <stdint.h>

// SM4 轮密钥
typedef uint32_t* SM4_Key;

 // 给定 128bit 主密钥，生成 SM4 轮密钥。执行成功返回1，失败返回0。
void SM4_KeyInit(uint8_t* key, SM4_Key sm4_key);

// 输入 128 * 8 bit 明文, 输出 128 * 8 bit 密文，内存可重叠
void SM4_Encrypt(uint8_t* plaintext, uint8_t* ciphertext, SM4_Key sm4_key);

// 输入 128 * 8 bit 密文, 输出 128 * 8 bit 明文，内存可重叠
void SM4_Decrypt(uint8_t* ciphertext, uint8_t* plaintext, SM4_Key sm4_key);

// 释放 SM4 轮密钥内存空间
void SM4_KeyDelete(SM4_Key sm4_key);

bool SelfTest();