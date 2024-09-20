//
// Created by 赵睿智 on 2022/4/21.
//

#ifndef CRYPTO_EXPERIMENT_STREAM_CIPHER_H
#define CRYPTO_EXPERIMENT_STREAM_CIPHER_H

// 定义MT19937-32的常数
enum
{
    // 假定 W = 32 (此项省略)
    N = 624,
    M = 397,
    R = 31,
    A = 0x9908B0DF,

    F = 1812433253,

    U = 11,
    // 假定 D = 0xFFFFFFFF (此项省略)

    S = 7,
    B = 0x9D2C5680,

    T = 15,
    C = 0xEFC60000,

    L = 18,

    MASK_LOWER = (1ull << R) - 1,
    MASK_UPPER = (1ull << R)
};

#include <gmp.h>
#include <stdint.h>

//BBS随机数发生器
void BBS_random(mpz_t *ans, int digit, mpz_t p, mpz_t q, mpz_t s);

//梅森素数旋转算法
static uint32_t MT[N];
static uint16_t Index;
uint32_t* Mersenne_Twister(uint32_t seed, int num);
void seed_Init(uint32_t seed);
void twist();
uint32_t extract();

//祖冲之密钥生成
void LFSR_init_Mode(uint32_t (*reg)[16], uint32_t u);
void LFSR_work_Mode(uint32_t (*reg)[16]);
void BR(uint32_t (*X)[4], const uint32_t reg[16]);
void ZUC_F(uint32_t *W, uint32_t X0, uint32_t X1, uint32_t X2);
void store_key(uint32_t (*reg)[16], const uint32_t key[16], const uint32_t IV[16]);
void ZUC_getkey(uint32_t (*key)[16], const char *hex_32byte_str);
void ZUC_getIV(uint32_t (*IV)[16], const char *hex_32byte_str);
void ZUC(char *key_str, char *IV_str, int round);


#endif //CRYPTO_EXPERIMENT_STREAM_CIPHER_H
