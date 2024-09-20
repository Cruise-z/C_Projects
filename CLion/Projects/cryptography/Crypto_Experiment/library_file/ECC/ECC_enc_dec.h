//
// Created by 赵睿智 on 2022/5/13.
//

#ifndef CRYPTO_EXPERIMENT_ECC_ENC_DEC_H
#define CRYPTO_EXPERIMENT_ECC_ENC_DEC_H

#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include "../conversion/lib_Convert.h"
#include "../RSA_Cipher/RSA_enc_dec.h"

typedef struct point_inf{
    mpz_t x;
    mpz_t y;
    mpz_t order;
}point, *p_ptr;

typedef struct Jacobi_point{
    mpz_t x;
    mpz_t y;
    mpz_t z;
}jp, *jp_ptr;

typedef struct EC_inf{
    mpz_t p; //大素数
    mpz_t a;
    mpz_t b;
}EC, *EC_ptr;

typedef struct Cipher_point{
    point C1;
    point C2;
}Cp, *Cp_ptr;

void init_point(p_ptr p);
void init_set_point_ul(p_ptr p, uint64_t src_x, uint64_t src_y);
void assign_point(p_ptr des, point src);
void clear_point(p_ptr des);
void init_EC(EC_ptr ptr);
void init_Cp(Cp_ptr ptr);
bool get_lambda(mpz_t *ans, point p, point q, EC C);
void Add_point(p_ptr ans, point P, point Q, EC C);
void Sub_point(p_ptr ans, point p, point q, EC C);
void sMul_point(p_ptr ans, point p, mpz_t k, EC C);
void sDiv_point(p_ptr ans, point p, mpz_t k, EC C);
void ECC_PK_enc(Cp_ptr ans, point A_Plaintext_point, mpz_t rand, point B_PK_point, EC C, point G);
void ECC_PK_fast_enc_pre(Cp_ptr pre, mpz_t rand, point B_PK_point, EC C, point G);
void ECC_PK_fast_enc(Cp_ptr ans, point A_Plaintext_point, Cp pre, EC C);
void ECC_PK_dec(p_ptr ans, Cp ciphertext_point, mpz_t B_SK, EC C);
void ECC_Pk_fast_dec_pre(p_ptr pre, point cp1, mpz_t B_SK, EC C);
void ECC_Pk_fast_dec(p_ptr ans, Cp ciphertext_point, point pre, EC C);

void init_jpoint(jp_ptr p);
void assign_jpoint(jp_ptr des, jp src);
void clear_jpoint(jp_ptr des);
void convert_jp2p(p_ptr des, jp src, EC c);
void convert_p2jp(jp_ptr des, point src, EC c);
void mmadd_2007_bl(jp_ptr ans, jp p, jp q, EC c);
void add_2007_bl(jp_ptr ans, jp p, jp q, EC c);
void mdbl_2007_bl(jp_ptr ans, jp p, EC c);
void dbl_2007_bl(jp_ptr ans, jp p, EC c);
void sub_jpoint(jp_ptr ans, jp p, jp q, EC c);
void slid_window_smul1(p_ptr ans, point P, mpz_t k, EC c);

int KDF_SHA256(bit_ptr K, bit_arr Z, uint64_t k_len);
void ECC_encode(mpz_t *re, byte_arr M, mpz_t k, uint64_t Par, point G, point Pb, EC C);
void ECC_decode(byte_ptr re, byte_arr M, uint64_t Par, point G, mpz_t Db, EC C);

#endif //CRYPTO_EXPERIMENT_ECC_ENC_DEC_H
