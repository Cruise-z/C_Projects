//
// Created by 赵睿智 on 2022/4/29.
//

#ifndef CRYPTO_EXPERIMENT_RSA_ENC_DEC_H
#define CRYPTO_EXPERIMENT_RSA_ENC_DEC_H

#include <gmp.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include "../My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../conversion/lib_Convert.h"

#define LEN 2048

//低解密指数攻击
void con_frac(mpz_t buf[LEN], int *len, mpz_t N, mpz_t D);
void get_frac(mpz_t (*frac)[LEN][2], mpz_t buf[LEN], int len);
int solve_x2_bx_c_integer(mpz_t *x1, mpz_t *x2, mpz_t b, mpz_t c);
void Wiener_Attack(mpz_t *p, mpz_t *q, mpz_t *d, mpz_t e, mpz_t N);

void PkSk2pq_Attack(mpz_t *p, mpz_t *q, mpz_t e, mpz_t d, mpz_t N);

void CM_Attack(mpz_t *message, mpz_t e1, mpz_t e2, mpz_t c1, mpz_t c2, mpz_t N);

void LEIB_Attack(mpz_t *message, mpz_t src_r[LEN], mpz_t src_p[LEN], int group_num);

int MGF1_SHA1(byte_ptr ans_ptr, byte_arr mgfSeed, int64_t maskLen);
int OAEP_SHA1(mpz_t *re, int64_t k, char *str_M, char *str_L, char *str_Seed, char mode, char *str_eORd, char *str_N);

void RSA_Workmode();


#endif //CRYPTO_EXPERIMENT_RSA_ENC_DEC_H
