#pragma once

#include <stdint.h>

// SM4 ����Կ
typedef uint32_t* SM4_Key;

 // ���� 128bit ����Կ������ SM4 ����Կ��ִ�гɹ�����1��ʧ�ܷ���0��
void SM4_KeyInit(uint8_t* key, SM4_Key sm4_key);

// ���� 128 * 8 bit ����, ��� 128 * 8 bit ���ģ��ڴ���ص�
void SM4_Encrypt(uint8_t* plaintext, uint8_t* ciphertext, SM4_Key sm4_key);

// ���� 128 * 8 bit ����, ��� 128 * 8 bit ���ģ��ڴ���ص�
void SM4_Decrypt(uint8_t* ciphertext, uint8_t* plaintext, SM4_Key sm4_key);

// �ͷ� SM4 ����Կ�ڴ�ռ�
void SM4_KeyDelete(SM4_Key sm4_key);

bool SelfTest();