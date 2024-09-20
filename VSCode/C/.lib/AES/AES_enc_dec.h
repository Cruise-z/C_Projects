//
// Created by 赵睿智 on 2022/3/31.
//

#ifndef CRYPTO_EXPERIMENT_AES_ENC_DEC_H
#define CRYPTO_EXPERIMENT_AES_ENC_DEC_H

#include <stdbool.h>

extern int AES_S_box[16][16];
extern int AES_inv_S_box[16][16];
extern int Rcon[31];

#define LEN 2048

//该结构体用来储存密钥块一个位置注入错误所生成的全部密文
typedef struct Attack_wrong_cipher_inf{
    char cipher[LEN][35];
    int num;  //该变量用来计数储存错误密文的个数
}wc_inf, *wc_inf_ptr;

void str2block(int (*re_block)[4][4], char *hex_32bit_str);
void S_box_substitution(int (*re_S_block)[4][4], int block[4][4]);
void str2key(int (*key_table)[4][60], char *hex_key);
void Extend_key(int (*key_table)[4][60], int key_type);
void Shift_row(int (*re_shift_block)[4][4], int S_block[4][4]);
void inv_Shift_row(int (*re_shift_block)[4][4], int S_block[4][4]);
int g_mul(int a, int poly);
int GF2_mul(int a1, int a2, int poly);
void Mix_column(int (*re_mix_block)[4][4], int shift_block[4][4]);
void inv_Mix_column(int (*re_mix_block)[4][4], int shift_block[4][4]);
void Add_round_key(int (*block)[4][4], int mix_block[4][4], const int key_table[4][60], int round);
void round_AES(int (*re_iteration_block)[4][4], const int key_table[4][60], int type, char mode);
void output_block(int block[4][4]);
void output_key(int key[4][60]);

void get_prob_byte(int (*key10_temp)[4][4][256], int correct_cipher_block[4][4], int row, int column, char wrong_cipher[35]);
int check_byte(const int byte_temp[256]);
void inv_Extend_key_128(int (*key_board)[4][44]);
void get_key_128(int (*key_board)[4][44], int correct_cipher_block[4][4], wc_inf wrong_cipher[4][4], char mode);

#endif //CRYPTO_EXPERIMENT_AES_ENC_DEC_H
