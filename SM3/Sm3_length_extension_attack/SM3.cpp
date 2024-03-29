#include "SM3.hpp"

// 消息扩展函数，通过 Bi[16] 计算 W[64]
void BiToW(unsigned int Bi[], unsigned int W[])
{
    int i;
    unsigned int tmp;
    for (i = 0; i <= 15; i++) {
        W[i] = Bi[i];
    }
    for (i = 16; i <= 67; i++) {
        tmp = W[i - 16] ^ W[i - 9] ^ SM3_rotl32(W[i - 3], 15);
        W[i] = SM3_p1(tmp) ^ (SM3_rotl32(W[i - 13], 7)) ^ W[i - 6];
    }
}

// 根据已生成的 W[64] 生成 W1[64]
void WToW1(unsigned int W[], unsigned int W1[])
{
    int i;
    for (i = 0; i <= 63; i++) {
        W1[i] = W[i] ^ W[i + 4];
    }
}

// 压缩函数，通过W[64] W1[64] V[8] 更新 V[8]
void CF(unsigned int W[], unsigned int W1[], unsigned int V[])
{
    unsigned int SS1;
    unsigned int SS2;
    unsigned int TT1;
    unsigned int TT2;
    unsigned int A, B, C, D, E, F, G, H;
    unsigned int T = SM3_T1;
    unsigned int FF;
    unsigned int GG;
    int j;
    //初始化寄存器，设置 ABCDEFGH=V0
    A = V[0]; B = V[1]; C = V[2]; D = V[3]; E = V[4]; F = V[5]; G = V[6]; H = V[7];

    /*
    for (int i = 0; i < 8; i++) {
        printf("%08x ", V[i]);
    }printf("\n");
    */

    for (j = 0; j <= 63; j++) {
        //生成中间变量 SS1
        if (j == 0) T = SM3_T1;
        else if (j == 16) T = SM3_rotl32(SM3_T2, 16);
        else T = SM3_rotl32(T, 1);

        SS1 = SM3_rotl32((SM3_rotl32(A, 12) + E + T), 7);

        //生成中间变量 SS2
        SS2 = SS1 ^ SM3_rotl32(A, 12);

        //生成中间变量 TT1
        if (j <= 15) FF = SM3_ff0(A, B, C);
        else FF = SM3_ff1(A, B, C);

        TT1 = FF + D + SS2 + *W1;
        W1++;

        //生成中间变量 TT2
        if (j <= 15) GG = SM3_gg0(E, F, G);
        else GG = SM3_gg1(E, F, G);

        TT2 = GG + H + SS1 + *W;
        W++;
        D = C;
        C = SM3_rotl32(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = SM3_rotl32(F, 19);
        F = E;
        E = SM3_p0(TT2);
    }
    //更新V的值
    V[0] = A ^ V[0]; V[1] = B ^ V[1]; V[2] = C ^ V[2]; V[3] = D ^ V[3];
    V[4] = E ^ V[4]; V[5] = F ^ V[5]; V[6] = G ^ V[6]; V[7] = H ^ V[7];
}

// 把小端转化成大端
void BigEndian(unsigned char src[], unsigned int bytelen, unsigned char dst[])
{
    unsigned char tmp = 0;
    unsigned int i = 0;
    for (i = 0; i < bytelen / 4; i++) {
        tmp = dst[4 * i];
        dst[4 * i] = src[4 * i + 3];
        src[4 * i + 3] = tmp;
        tmp = dst[4 * i + 1];
        dst[4 * i + 1] = src[4 * i + 2];
        dst[4 * i + 2] = tmp;
    }
}

// SM3 状态初始化
void SM3_init(SM3_STATE* md)
{
    md->curlen = md->length = 0;
    md->state[0] = SM3_IVA; md->state[1] = SM3_IVB;
    md->state[2] = SM3_IVC; md->state[3] = SM3_IVD;
    md->state[4] = SM3_IVE; md->state[5] = SM3_IVF;
    md->state[6] = SM3_IVG; md->state[7] = SM3_IVH;
}

// 压缩单块信息
void SM3_compress(SM3_STATE* md)
{
    unsigned int W[68];
    unsigned int W1[64];
    BigEndian(md->buf, 64, md->buf);   //转化大小端
    BiToW((unsigned int*)md->buf, W);
    WToW1(W, W1);
    CF(W, W1, md->state);
}

// 压缩前 (len/64) 个块的消息
void SM3_process(SM3_STATE* md, unsigned char* buf, int len)
{
    while (len--) {
        // 复制字节
        md->buf[md->curlen] = *buf++;
        md->curlen++;
        // 是否达到了64字节
        if (md->curlen == 64) {
            SM3_compress(md);
            md->length += 512;
            md->curlen = 0;
        }
    }
}

// 压缩剩余的消息
void SM3_done(SM3_STATE* md, unsigned char hash[])
{
    int i;
    unsigned char tmp = 0;
    md->length += md->curlen << 3;  // 增加消息长度
    md->buf[md->curlen] = 0x80;    // 末尾加 '1'
    md->curlen++;
    // 如果最后一块消息长度超过56B，直接全补0压缩
    if (md->curlen > 56) {
        while (md->curlen < 64) {
            md->buf[md->curlen] = 0;
            md->curlen++;
        }
        SM3_compress(md);
        md->curlen = 0;
    }
    // 如果最后一块消息长度不到56B，补0到56B
    while (md->curlen < 56) {
        md->buf[md->curlen] = 0;
        md->curlen++;
    }
    // 因为一个消息一般不超过 2^32 bit 所以直接置0
    for (i = 56; i < 60; i++) {
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

void SM3_256(unsigned char buf[], int len, unsigned char hash[])
{
    SM3_STATE md;
    SM3_init(&md);
    SM3_process(&md, buf, len);
    SM3_done(&md, hash);
}

// 使用标准数据对 SM3 程序进行正确性测试
int SM3_SelfTest()
{
    unsigned int i = 0, a = 1, b = 1;
    unsigned char Msg1[3] = { 0x61,0x62,0x63 };
    int MsgLen1 = 3;
    unsigned char MsgHash1[32] = { 0 };
    unsigned char StdHash1[32] = { 0x66,0xC7,0xF0,0xF4,0x62,0xEE,0xED,0xD9,0xD1,0xF2,0xD4,0x6B,0xDC,0x10,0xE4,0xE2,0x41,0x67,0xC4,0x87,0x5C,0xF2,0xF7,0xA2,0x29,0x7D,0xA0,0x2B,0x8F,0x4B,0xA8,0xE0 };
    unsigned char Msg2[64] = { 0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,
                            0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,
                            0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,
                            0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64 };
    int MsgLen2 = 64;
    unsigned char MsgHash2[32] = { 0 };
    unsigned char StdHash2[32] = { 0xde,0xbe,0x9f,0xf9,0x22,0x75,0xb8,0xa1,0x38,0x60,0x48,0x89,0xc1,0x8e,0x5a,0x4d,0x6f,0xdb,0x70,0xe5,0x38,0x7e,0x57,0x65,0x29,0x3d,0xcb,0xa3,0x9c,0x0c,0x57,0x32 };
    SM3_256(Msg1, MsgLen1, MsgHash1);
    SM3_256(Msg2, MsgLen2, MsgHash2);
    a = memcmp(MsgHash1, StdHash1, SM3_len / 8);
    b = memcmp(MsgHash2, StdHash2, SM3_len / 8);
    if ((a == 0) && (b == 0)) return 0;
    else return 1;
}