//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <openssl/evp.h>
#include <random>
#include <ctime>
using namespace std;
#define TIMING

#define SM3_MAX_SIZE 32
#define MSG_MAX_LEN 64

char CHARSET[63] = {
	'0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J',
	'K','L','M','N','O','P','Q','R','S','T',
	'U','V','W','X','Y','Z','a','b','c','d',
	'e','f','g','h','i','j','k','l','m','n',
	'o','p','q','r','s','t','u','v','w','x',
	'y','z','\0'
};

struct Collision_pair {
	uint8_t msg1[SM3_MAX_SIZE], msg2[SM3_MAX_SIZE];
	uint8_t dgst1[SM3_MAX_SIZE], dgst2[SM3_MAX_SIZE];
}result;

// 利用随机数随机生成消息
string get_msg() {
	static mt19937 rnd(time(0));
	static uniform_int_distribution<> range(0, 61);
	string res = "";
	for (int i = 0; i < MSG_MAX_LEN; i++) {
		// 生成 [0, 61] 范围的值作为索引
		res += CHARSET[range(rnd)];
	}
	return res;
}

bool sm3_openssl(uint8_t* dgst, const void* msg, size_t len) {
	bool res = false;
	const EVP_MD* md = EVP_get_digestbyname("sm3");
	EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
	if (!mdctx) goto done;

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, msg, len);  //调用Update计算message的hash值
	res = EVP_DigestFinal_ex(mdctx, dgst, NULL);

done:
	EVP_MD_CTX_free(mdctx);
	return res;
}

// 输出结果
void ShowResult(int num_of_bit) {
	cout << "Message 1  (Hex): ";
	for (int i = 0; i < num_of_bit / 8; i++) {
		printf("%02x", result.msg1[i]);
	}
	cout << "\nHash value (Hex): ";
	for (int i = 0; i < SM3_MAX_SIZE; i++) {
		printf("%02x", result.dgst1[i]);
	}
	cout << "\nMessage 2  (Hex): ";
	for (int i = 0; i < num_of_bit / 8; i++) {
		printf("%02x", result.msg2[i]);
	}
	cout << "\nHash value (Hex): ";
	for (int i = 0; i < SM3_MAX_SIZE; i++) {
		printf("%02x", result.dgst2[i]);
	}
	printf("\n");
}

void FindCollision(int num_of_bit)
{
	uint64_t count = 0, max_iter_time = (1ull << num_of_bit / 2);
	string msg = get_msg();
	uint8_t x0[SM3_MAX_SIZE], x1[SM3_MAX_SIZE], x2[SM3_MAX_SIZE];

#ifdef TIMING
	clock_t start = clock();
#endif

	sm3_openssl(x0, msg.c_str(), MSG_MAX_LEN);
	memcpy(x1, x0, SM3_MAX_SIZE);
	memcpy(x2, x0, SM3_MAX_SIZE);

	while (true) {
		sm3_openssl(x1, x1, num_of_bit / 8);
		sm3_openssl(x2, x2, num_of_bit / 8);
		sm3_openssl(x2, x2, num_of_bit / 8);
		count++;
		if (memcmp(x1, x2, num_of_bit / 8) == 0) {
			break;
		}
		if (count == max_iter_time) {
			cout << "Failed, excessive number of iterations.\n";
			return;
		}
	}

	uint8_t tmp1[SM3_MAX_SIZE], tmp2[SM3_MAX_SIZE];
	memcpy(x2, x1, SM3_MAX_SIZE);
	memcpy(x1, x0, SM3_MAX_SIZE);
	
	for (int i = 0; i < count; i++) {
		if (memcmp(x1, x2, num_of_bit / 8) == 0) {
#ifdef TIMING
			clock_t end = clock();
			cout << "Time cost: " << double(end - start) / CLOCKS_PER_SEC << "s\n";
#endif
			memcpy(result.dgst1, x1, SM3_MAX_SIZE);
			memcpy(result.dgst2, x2, SM3_MAX_SIZE);
			memcpy(result.msg1, tmp1, num_of_bit / 8);
			memcpy(result.msg2, tmp2, num_of_bit / 8);
			ofstream out("../a.txt");  // 将结果分别写入文件 a.txt 和 b.txt
			if (!out) {
				cout << "Cannot open file";
				return;
			}
			out.write((char*)&tmp1, num_of_bit / 8);
			out.close();

			out.open("../b.txt");
			if (!out) {
				cout << "Cannot open file";
				return;
			}
			out.write((char*)&tmp2, num_of_bit / 8);
			out.close();
			break;
		}
		memcpy(tmp1, x1, SM3_MAX_SIZE);
		memcpy(tmp2, x2, SM3_MAX_SIZE);
		sm3_openssl(x1, x1, num_of_bit / 8);
		sm3_openssl(x2, x2, num_of_bit / 8);
	}
	cout << "After generating " << count << " messages, find the collision:\n";
	ShowResult(num_of_bit);
}

int main()
{
	FindCollision(56);
	return 0;
}
