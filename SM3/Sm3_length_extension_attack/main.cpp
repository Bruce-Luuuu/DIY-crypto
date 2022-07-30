#include "SM3.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <random>
using namespace std;

#define MSG_LEN 55
#define SM3_OUTPUT_LEN 32

char CHARSET[63] = {
	'0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J',
	'K','L','M','N','O','P','Q','R','S','T',
	'U','V','W','X','Y','Z','a','b','c','d',
	'e','f','g','h','i','j','k','l','m','n',
	'o','p','q','r','s','t','u','v','w','x',
	'y','z','\0'
};

string get_msg() {
	static mt19937 rnd(time(0));
	static uniform_int_distribution<> range(0, 61);
	string res = "";
	for (int i = 0; i < MSG_LEN; i++) {
		res += CHARSET[range(rnd)]; // 生成 [0, 61] 范围的值作为索引
	}
	return res;
}

void SM3_fake(SM3_STATE* md, unsigned char buf[], int len, unsigned char hash[])
{
	SM3_process(md, buf, len);
	SM3_done(md, hash);
}

void show(uint8_t* array, int len) {
	for (int i = 0; i < len; i++) {
		printf("%02x", array[i]);
	}
	printf("\n");
}

int main() {
	/* 验证 SM3 程序正确性
	if (SM3_SelfTest()) {
		cout << "SM3 is not correct";
		exit(0);
	} */
	uint8_t dgst_M[SM3_OUTPUT_LEN];
	uint8_t dgst_M_pad_M1[SM3_OUTPUT_LEN];
	uint8_t dgst_fake[SM3_OUTPUT_LEN];
	uint8_t M[MSG_LEN];
	uint8_t M1[MSG_LEN];
	uint8_t M_pad_M1[64 + MSG_LEN];
	
	memcpy(M, get_msg().c_str(), MSG_LEN);
	SM3_256(M, MSG_LEN, dgst_M);
	
	show(M, MSG_LEN);
	show(dgst_M, SM3_OUTPUT_LEN);
	
	cout << "-------------------------------------------------------------------------\n";

	SM3_STATE md;
	md.curlen = md.length = 0;
	// 大小端？memcpy(md.state, dgst_M, 32);
	///*
	memcpy(md.state, dgst_M, 4);
	memcpy(md.state + 1, dgst_M + 4, 4);
	memcpy(md.state + 2, dgst_M + 8, 4);
	memcpy(md.state + 3, dgst_M + 12, 4);
	memcpy(md.state + 4, dgst_M + 16, 4);
	memcpy(md.state + 5, dgst_M + 20, 4);
	memcpy(md.state + 6, dgst_M + 24, 4);
	memcpy(md.state + 7, dgst_M + 28, 4);
	//*/
	/*
	memcpy(md.state, dgst_M + 28, 4);
	memcpy(md.state + 1, dgst_M + 24, 4);
	memcpy(md.state + 2, dgst_M + 20, 4);
	memcpy(md.state + 3, dgst_M + 16, 4);
	memcpy(md.state + 4, dgst_M + 12, 4);
	memcpy(md.state + 5, dgst_M + 8, 4);
	memcpy(md.state + 6, dgst_M + 4, 4);
	memcpy(md.state + 7, dgst_M, 4);
	*/
	/*
	md.state[0] = SM3_IVA; 
	md.state[1] = SM3_IVB;
	md.state[2] = SM3_IVC; 
	md.state[3] = SM3_IVD;
	md.state[4] = SM3_IVE; 
	md.state[5] = SM3_IVF;
	md.state[6] = SM3_IVG; 
	md.state[7] = SM3_IVH;
	*/

	memcpy(M1, get_msg().c_str(), MSG_LEN);
	SM3_fake(&md, M1, MSG_LEN, dgst_fake);

	show(M1, MSG_LEN);
	show(dgst_fake, SM3_OUTPUT_LEN);

	// 计算 M_pad_M1 的哈希值验证是否伪造成功
	cout << "-------------------------------------------------------------------------\n";
	memcpy(M_pad_M1, M, MSG_LEN);
	// 添加 padding
	M_pad_M1[55] = 0x80;
	for (int i = 56; i < 60; i++) {
		M_pad_M1[i] = 0;
	}
	uint32_t length = 55;
	M_pad_M1[63] = length & 0xff;
	M_pad_M1[62] = (length >> 8) & 0xff;
	M_pad_M1[61] = (length >> 16) & 0xff;
	M_pad_M1[60] = (length >> 24) & 0xff;

	memcpy(M_pad_M1+64, M1, MSG_LEN);
	SM3_256(M_pad_M1, 64+MSG_LEN, dgst_M_pad_M1);

	show(M_pad_M1, 64+MSG_LEN);
	show(dgst_M_pad_M1, SM3_OUTPUT_LEN);
	return 0;
}