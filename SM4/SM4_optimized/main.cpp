#include "SM4.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <ctime>
#include <string>
using namespace std;

char CHAR_SET[63] = {
    '0','1','2','3','4','5','6','7','8','9',
    'A','B','C','D','E','F','G','H','I','J',
    'K','L','M','N','O','P','Q','R','S','T',
    'U','V','W','X','Y','Z','a','b','c','d',
    'e','f','g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v','w','x',
    'y','z','\0'
};

string GenMsg(string filename, uint32_t len) {
    static mt19937 rnd(time(0));
    static uniform_int_distribution<> range(0, 61);
    string res = "";
    for (uint32_t i = 0; i < len; i++) {
        res += CHAR_SET[range(rnd)];
    }
    return res;
}


int main() {
    if (SelfTest() == false) {
        cout << "SM4 implementation is not correct\n";
        exit(0);
    }
    unsigned char key[16 * 8] = { 0 };
    unsigned char in[16 * 8] = { 0 };
    SM4_Key sm4_key = (uint32_t*)malloc(32 * sizeof(uint32_t));
    // malloc ÉêÇë¿Õ¼äÊ§°Ü
    if (sm4_key == NULL) {
        cout << "malloc failed.\n";
    }

    SM4_KeyInit(key, sm4_key);
    SM4_Encrypt(in, in, sm4_key);
    SM4_Decrypt(in, in, sm4_key);
    SM4_KeyDelete(sm4_key);

    return 0;
}