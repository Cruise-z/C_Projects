//
// Created by 赵睿智 on 2022/3/16.
//

#ifndef CRYPTO_EXPERIMENT_MY_BIGNUM_GF_CAL_H
#define CRYPTO_EXPERIMENT_MY_BIGNUM_GF_CAL_H

#include <gmp.h>
#include <stdbool.h>

#define LEN 2048

typedef struct div_result{
    long long int ans;
    long long int remain;
}div_re, *div_ptr;

typedef struct gcd_result{
    long long int gcd;
    long long int x;
    long long int y;
}gcd_re, *gcd_ptr;

typedef struct mpz_div_result{
    char *ans;
    char *remain;
}mpz_div_re, *mpz_div_ptr;

typedef struct mpz_gcd_result{
    char *gcd_str;
    char *x_str;
    char *y_str;
    mpz_t gcd, x, y;
}mpz_gcd_re, *mpz_gcd_ptr;

//GF域中的相关运算
int gcd(int a, int b);
char xor(char c1, char c2);
char *standard(char *binary_s);
char *GF_2_xor(char *str1,char *str2);
mpz_div_re GF_2_poly_division(char *binary_dividend, char *binary_divisor);
char *GF_2_poly_multiplication(char *binary_s1, char *binary_s2);
char *GF_2_add_sub(char *binary_s1, char *binary_s2, char *binary_modstr);
char *GF_2_mul(char *binary_s1, char *binary_s2, char *binary_modstr);
mpz_div_re GF_2_hex_cal(char op, char *hex_s1, char *hex_s2, char *hex_modstr);
char *GF_2_hex_Fp(char *hex_b, char *dec_p, char *hex_modstr);
mpz_gcd_re GF_2_exEuclid(char *binary_s1, char *binary_s2, char *modstr);
mpz_gcd_re GF_2_hex_exEuclid(char *hex_s1, char *hex_s2, char *hex_modstr);

div_re Div(long long int pm, long long int mod);
gcd_re ex_gcd(long long int a, long long int b);

//const char hex_table[] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

//RSA加密解密的相关函数
mpz_gcd_re gcd_ext(mpz_t A1, mpz_t A2);
mpz_gcd_re str_gcdext(char *s1, char *s2);
void Eeatosthese(int num);
void Fp(mpz_t *ans, mpz_t B, mpz_t P, mpz_t M);
void CRT_RSA(mpz_t * c,mpz_t * p,mpz_t * q,mpz_t * d,mpz_t * ans);
void CRT(mpz_t *r, mpz_t *p, mpz_t src_r, mpz_t src_p);
void get_prime(mpz_t *prime, int size);
bool get_strong_prime(mpz_t *key,int bit);
bool rabin_wit(mpz_t key);
void randrange(mpz_t * randnum, mpz_t start, mpz_t end);
void mpz_pow(mpz_t * B,mpz_t * P,mpz_t * ans);
void inversion(mpz_t N, mpz_t M, mpz_t *rea, mpz_t *reb);
//pem编解码用到的函数
unsigned char *base64_encode(unsigned char *str);
unsigned char *base64_decode(unsigned char *code);
void to_utf8(char *tar);

//以下为模拟蒙特卡洛法实现取摸操作，但事实上mpz_mod在实现时已经采用了该方法进行简化



#endif //CRYPTO_EXPERIMENT_MY_BIGNUM_GF_CAL_H
