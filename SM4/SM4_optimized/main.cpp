#include "SM4.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

#define Bytes_per_Block 16

char CHAR_SET[63] = {
    '0','1','2','3','4','5','6','7','8','9',
    'A','B','C','D','E','F','G','H','I','J',
    'K','L','M','N','O','P','Q','R','S','T',
    'U','V','W','X','Y','Z','a','b','c','d',
    'e','f','g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v','w','x',
    'y','z','\0'
};

string GenStr(uint32_t len) {
    static mt19937 rnd(time(0));
    static uniform_int_distribution<> range(0, 61);
    string res = "";
    for (uint32_t i = 0; i < len; i++) {
        res += CHAR_SET[range(rnd)];
    }
    return res;
}

void test(int block_num) {
    uint8_t* key = (uint8_t*)malloc(16*sizeof(uint8_t));
    uint8_t* plaintext = (uint8_t*)malloc(block_num * Bytes_per_Block * sizeof(uint8_t));

    uint8_t* ciphertext = plaintext; // 重用变量以节约空间
    memcpy(key, GenStr(16).c_str(), 16);
    memcpy(plaintext, GenStr(block_num * Bytes_per_Block).c_str(), block_num * Bytes_per_Block);

    SM4_Key sm4_key = (uint32_t*)malloc(32 * sizeof(uint32_t));
    if (sm4_key == NULL) {
        cout << "malloc failed.\n";
        return;
    }

    SM4_KeyInit(key, sm4_key);
    uint32_t time = 0;
    clock_t start, end, whole = 0;
    while (true) {
        start = clock();
        for (int i = 0; i < block_num / 8; i++) {
            SM4_Encrypt((plaintext + 8 * Bytes_per_Block * i), (ciphertext + 8 * Bytes_per_Block * i), sm4_key);
        }
        end = clock();
        whole += (end - start);
        if (double(whole) / CLOCKS_PER_SEC > 3.0) break;
        time++;
    }
    cout << "Doing SM4 encryption for " << time << " times on " << block_num << " message blocks: ";
    cout << "Time cost is " << double(whole) / CLOCKS_PER_SEC << "s\n";
    //SM4_Decrypt(ciphertext, plaintext, sm4_key);
    SM4_KeyDelete(sm4_key);
}


int main() {
    if (SelfTest() == false) {
        cout << "SM4 implementation is not correct\n";
        exit(0);
    }
    vector<int> test_list = { 16, 64, 256, 1024, 8192, 16384 };
    for (auto block_num : test_list) {
        test(block_num);
    }
    return 0;
}