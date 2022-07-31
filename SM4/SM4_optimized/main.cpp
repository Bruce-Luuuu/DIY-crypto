#include "SM4.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

int main() {
    // 01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
    unsigned char key[16 * 8] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98,
                                 0x76, 0x54, 0x32, 0x10 };
    // 01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
    // 00 00 ... 00
    unsigned char in[16 * 8] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                                0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
    SM4_Key sm4_key = (uint32_t*)malloc(32 * sizeof(uint32_t));
    // malloc ����ռ�ʧ��
    if (sm4_key == NULL) {
        cout << "malloc failed.\n";
    }
    SM4_KeyInit(key, sm4_key);

    SM4_Encrypt(in, in, sm4_key);
    // 68 1e df 34 d2 06 96 5e 86 b3 e9 4f 53 6e 42 46
    // 26 77 f4 6b 09 c1 22 cc 97 55 33 10 5b d4 a2 2a
    // 26 ...
    printf("C:\n");
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 16; i++) {
            printf("%02x ", in[i + 16 * j]);
        }
        printf("\n");
    }

    printf("P:\n");
    SM4_Decrypt(in, in, sm4_key);
    // 01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
    // 00 00 ... 00
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 16; i++) {
            printf("%02x ", in[i + 16 * j]);
        }
        printf("\n");
    }

    SM4_KeyDelete(sm4_key);
    return 0;
}