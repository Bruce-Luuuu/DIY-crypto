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

// 在已知 hash(M) 和 M 的长度的情形下添加 M1 伪造签名
void SM3_fake(SM3_STATE* md, unsigned char buf[], int len, unsigned char hash[])
{
	SM3_process(md, buf, len);
	
	md->length += (MSG_LEN + 64) << 3;  // 增加消息长度
	md->buf[md->curlen] = 0x80;    // 末尾加 '1'
	md->curlen++;
	for (int i = 56; i < 60; i++) {
		md->buf[i] = 0;
	}
	// 在末尾添加消息长度的二进制表示
	md->buf[63] = md->length & 0xff;
	md->buf[62] = (md->length >> 8) & 0xff;
	md->buf[61] = (md->length >> 16) & 0xff;
	md->buf[60] = (md->length >> 24) & 0xff;

	SM3_compress(md);
	memcpy(hash, md->state, SM3_len / 8);     // 拷贝输出
	BigEndian(hash, SM3_len / 8, hash);      //转换大小端
}

void show(uint8_t* array, int len) {
	for (int i = 0; i < len; i++) {
		printf("%02x", array[i]);
	}
	printf("\n");
}

int main() {
	/* 验证 SM3 程序正确性 */
	if (SM3_SelfTest()) {
		cout << "SM3 is not correct";
		exit(0);
	} 
	uint8_t dgst_M[SM3_OUTPUT_LEN];
	uint8_t dgst_M_pad_M1[SM3_OUTPUT_LEN];
	uint8_t dgst_fake[SM3_OUTPUT_LEN];
	uint8_t M[MSG_LEN];
	uint8_t M1[MSG_LEN];
	uint8_t M_pad_M1[64 + MSG_LEN];
	
	memcpy(M, get_msg().c_str(), MSG_LEN); // 随机生成消息 M
	SM3_256(M, MSG_LEN, dgst_M); // 计算 M 的哈希值 dgst_M
	
	cout << "M:\n";  show(M, MSG_LEN);  // 输出信息
	cout << "hash(M):\n";  show(dgst_M, SM3_OUTPUT_LEN);
	cout << "-------------------------------------------------------------------------\n";

	SM3_STATE md;
	md.curlen = md.length = 0;
	
	BigEndian(dgst_M, 32, dgst_M); // 一定要注意大小端问题！
	memcpy(md.state, dgst_M, 32); // 利用hash(M)的值作为IV

	memcpy(M1, get_msg().c_str(), MSG_LEN);  // 随机生成消息 M1(现实中这里是伪造的消息 M1)
	SM3_fake(&md, M1, MSG_LEN, dgst_fake);   // 伪造哈希值

	cout << "M1:\n";  show(M1, MSG_LEN);
	cout << "hash(M1) （IV是hash(M)）:\n";  show(dgst_fake, SM3_OUTPUT_LEN);
	cout << "-------------------------------------------------------------------------\n";

	// 计算 M_pad_M1 对应的正确哈希值，以验证是否伪造成功，先构造消息 M+pad+M1
	memcpy(M_pad_M1, M, MSG_LEN);
	// 添加 padding
	M_pad_M1[55] = 0x80;
	for (int i = 56; i < 60; i++) {
		M_pad_M1[i] = 0;
	}
	uint32_t length = 55 << 3; // 长度单位是bit！ 不是字节！
	M_pad_M1[63] = length & 0xff;
	M_pad_M1[62] = (length >> 8) & 0xff;
	M_pad_M1[61] = (length >> 16) & 0xff;
	M_pad_M1[60] = (length >> 24) & 0xff;

	memcpy(M_pad_M1+64, M1, MSG_LEN);
	SM3_256(M_pad_M1, 64+MSG_LEN, dgst_M_pad_M1);

	cout << "M||pad||M1:\n";  show(M_pad_M1, 64+MSG_LEN); // 输出结果与伪造值进行比对
	cout << "hash(M||pad||M1):\n";  show(dgst_M_pad_M1, SM3_OUTPUT_LEN);
	return 0;
}