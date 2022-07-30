//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <unordered_map>
#include <openssl/evp.h>
#include <random>
#include <ctime>
using namespace std;
#define TIMING

#define SM3_MAX_SIZE 32
#define MSG_MAX_LEN 32
#define NUM_OF_BIT 16
#define MAX_ITER_TIME 0x3ffffffull

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
	string msg1, msg2;
}result;

unordered_map<string, string> umap;

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

bool sm3_openssl(uint8_t* dgst, const void *msg, size_t len) {
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

bool InsertMsg(int num_of_bit) {
	string msg = get_msg();
	uint8_t dgst[SM3_MAX_SIZE];
	sm3_openssl(dgst, msg.c_str(), MSG_MAX_LEN);
	string key;
	key.assign(&dgst[0], &dgst[0] + SM3_MAX_SIZE);
	key = key.substr(0, num_of_bit / 8);

	unordered_map<string, string>::iterator iter = umap.find(key);
	if (iter == umap.end()) {
		umap[key] = msg;
		return true;
	}
	else {  // 找到碰撞，存储相应的一对消息
		result.msg1 = iter->second;
		result.msg2 = msg;
		return false;
	}
}

void ShowResult() {
	// 重新计算 找到的 1 对碰撞消息各自的哈希值
	uint8_t dgst1[SM3_MAX_SIZE], dgst2[SM3_MAX_SIZE];
	sm3_openssl(dgst1, result.msg1.c_str(), MSG_MAX_LEN);
	sm3_openssl(dgst2, result.msg2.c_str(), MSG_MAX_LEN);
	
	// 输出结果
	cout << "Message 1:  " << result.msg1 << '\n' << "Hash value: ";
	for (int i = 0; i < SM3_MAX_SIZE; i++) {
		printf("%02x", dgst1[i]);
	}
	printf("\n");
	cout << "Message 2:  " << result.msg2 << '\n' << "Hash value: ";
	for (int i = 0; i < SM3_MAX_SIZE; i++) {
		printf("%02x", dgst2[i]);
	}
	printf("\n");
}

void FindCollision(int num_of_bit)
{
	uint64_t count = 0;
	bool flag = true;

#ifdef TIMING
	clock_t start = clock();
#endif

	while(flag) {	// 不断生成消息，直到找到碰撞
		flag = InsertMsg(num_of_bit);
		count++;
		if (count == MAX_ITER_TIME) break;
	}

#ifdef TIMING
	clock_t end = clock();
	cout << "Time cost: " << double(end - start) / CLOCKS_PER_SEC << "s\n";
#endif
	// 展示碰撞结果
	cout << "After " << count << " messages are generated, find the collision:\n";
	ShowResult();
}

int main()
{
	// 寻找高位56比特碰撞
	FindCollision(56);
	return 0;
}
