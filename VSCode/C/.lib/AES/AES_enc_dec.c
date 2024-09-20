//
// Created by 赵睿智 on 2022/3/31.
//

#include "AES_enc_dec.h"
#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../My_Matrix_cal/My_Matrix_cal.h"
#include "../My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../conversion/lib_Convert.h"

int AES_S_box[16][16] = {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
                         0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                         0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
                         0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                         0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
                         0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                         0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
                         0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                         0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
                         0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                         0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
                         0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                         0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
                         0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                         0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
                         0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

int AES_inv_S_box[16][16] = {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
                             0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
                             0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
                             0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
                             0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
                             0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
                             0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
                             0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
                             0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
                             0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
                             0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
                             0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
                             0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
                             0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
                             0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
                             0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

int Rcon[31] = {      0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
                0x80, 0x1B, 0x36, 0x6C, 0xD8, 0xAB, 0x4D, 0x9A,
                0x2F, 0x5E, 0xBC, 0x63, 0xC6, 0x97, 0x35, 0x6A,
                0xD4, 0xB3, 0x7D, 0xFA, 0xEF, 0xC5, 0x91, 0x39};

void str2block(int (*re_block)[4][4], char *hex_32bit_str){
    hex_32bit_str += 2;
    int i, j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            (*re_block)[j][i] = hex2bin_index(hex_32bit_str[8*i+2*j])*16
                    + hex2bin_index(hex_32bit_str[8*i+2*j+1]);
}

void S_box_substitution(int (*re_S_block)[4][4], int block[4][4]){
    int i, j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            (*re_S_block)[i][j] = AES_S_box[block[i][j] >> 4][block[i][j] & 0xf];
}

void inv_S_box_substitution(int (*re_S_block)[4][4], int block[4][4]){
    int i, j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            (*re_S_block)[i][j] = AES_inv_S_box[block[i][j] >> 4][block[i][j] & 0xf];
}

void str2key(int (*key_table)[4][60], char *hex_key){
    hex_key += 2;
    int i, j, len = (int)strlen(hex_key);
    for(i = 0; i < len/8; i++)
        for(j = 0; j < 4; j++)
            (*key_table)[j][i] = hex2bin_index(hex_key[8*i+2*j])*16
                    + hex2bin_index(hex_key[8*i+2*j+1]);
}

void Extend_key(int (*key_table)[4][60], int key_type){
    int row, column;
    int step_len = key_type/32,                    //迭代步长
        word_sum = 44+(key_type-128)/8;            //所需字的总数
    int word_temp[4];
    for(column = step_len; column < word_sum; column++){
        if(step_len == 8 && column%8 == 4){
            for(row = 0; row < 4; row++)
                (*key_table)[row][column] =
                        AES_S_box[(*key_table)[row][column - 1] >> 4][(*key_table)[row][column - 1] & 0xf] ^
                        (*key_table)[row][column-step_len];
            continue;
        }
        if(column % step_len != 0)
            for(row = 0; row < 4; row++)
                (*key_table)[row][column] = (*key_table)[row][column-step_len] ^ (*key_table)[row][column-1];
        else{
            for(row = 0; row < 4; row++){
                word_temp[row] = (*key_table)[(row+1)%4][column - 1];
                (*key_table)[row][column] = AES_S_box[word_temp[row] >> 4][word_temp[row] & 0xf];
            }
            (*key_table)[0][column] = (*key_table)[0][column] ^ Rcon[column/step_len-1];
            for(row = 0; row < 4; row++)
                (*key_table)[row][column] = (*key_table)[row][column-step_len] ^ (*key_table)[row][column];
        }
    }
}

void Shift_row(int (*re_shift_block)[4][4], int S_block[4][4]){
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            (*re_shift_block)[i][j] = S_block[i][(j + i) % 4];
}

void inv_Shift_row(int (*re_shift_block)[4][4], int S_block[4][4]){
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            (*re_shift_block)[i][j] = S_block[i][(j + 4-i) % 4];
}

int g_mul(int a, int poly){
    a <<= 1;
    if((a & 0x100) == 0x100)
        a ^= poly;
    return a & 0xff;
}

int GF2_mul(int a1, int a2, int poly){
    int result = 0;
    while(a2 > 0){
        if((a2 & 0x1) == 1)
            result ^= a1;
        a1 = g_mul(a1, poly);
        a2 >>= 1;
    }
    return result;
}

void Mix_column(int (*re_mix_block)[4][4], int shift_block[4][4]){
    int row, column, temp1, temp2;
    for(column = 0; column < 4; column++){
        for(row = 0; row < 4; row++){
            temp1 = GF2_mul(0x2, shift_block[row%4][column], 0x11b);
            temp2 = GF2_mul(0x3, shift_block[(row+1)%4][column], 0x11b);
            (*re_mix_block)[row][column] = temp1 ^ temp2 ^ shift_block[(row+2)%4][column] ^ shift_block[(row+3)%4][column];
        }
    }
}

void inv_Mix_column(int (*re_mix_block)[4][4], int shift_block[4][4]){
    int row, column, temp1, temp2, temp3, temp4;
    for(column = 0; column < 4; column++){
        for(row = 0; row < 4; row++){
            temp1 = GF2_mul(0xe, shift_block[row%4][column], 0x11b);
            temp2 = GF2_mul(0xb, shift_block[(row+1)%4][column], 0x11b);
            temp3 = GF2_mul(0xd, shift_block[(row+2)%4][column], 0x11b);
            temp4 = GF2_mul(0x9, shift_block[(row+3)%4][column], 0x11b);
            (*re_mix_block)[row][column] = temp1 ^ temp2 ^ temp3 ^ temp4;
        }
    }
}

void Add_round_key(int (*block)[4][4], int mix_block[4][4], const int key_table[4][60], int round){
    int i, j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            (*block)[i][j] = mix_block[i][j] ^ key_table[i][round*4+j];
}

void round_AES(int (*re_iteration_block)[4][4], const int key_table[4][60], int type, char mode){
    int round = 10 + (type - 128) / 32;
    if(mode == 'e'){
        int block[4][4], s_block[4][4], sr_block[4][4], mc_block[4][4], round_cnt;
        Add_round_key(&block, *re_iteration_block, key_table, 0);
        for(round_cnt = 1; round_cnt < round; round_cnt++){
            S_box_substitution(&s_block, block);
            Shift_row(&sr_block, s_block);
            Mix_column(&mc_block, sr_block);
            Add_round_key(&block, mc_block, key_table, round_cnt);
        }
        S_box_substitution(&s_block, block);
        Shift_row(&sr_block, s_block);
        Add_round_key(re_iteration_block, sr_block, key_table, round_cnt);
    }else if(mode == 'd'){
        int block[4][4], inv_sr_block[4][4], inv_s_block[4][4], add_round_block[4][4], round_cnt;
        Add_round_key(&block, *re_iteration_block, key_table, round);
        for(round_cnt = round-1; round_cnt > 0; round_cnt--){
            inv_Shift_row(&inv_sr_block, block);
            inv_S_box_substitution(&inv_s_block, inv_sr_block);
            Add_round_key(&add_round_block, inv_s_block, key_table, round_cnt);
            inv_Mix_column(&block, add_round_block);
        }
        inv_Shift_row(&inv_sr_block, block);
        inv_S_box_substitution(&inv_s_block, inv_sr_block);
        Add_round_key(re_iteration_block, inv_s_block, key_table, round_cnt);
    }
}

void output_block(int block[4][4]){
    printf("0x");
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            printf("%02x", block[j][i]);
    printf("\n");
}

void output_key(int key[4][60]){
    for(int i = 0; i < 60; i++){
        printf("%d:", i);
        for(int j = 0; j < 4; j++){
            printf("%02x", key[j][i]);
        }
        printf("\n");
    }
}

void get_prob_byte(int (*key10_temp)[4][4][256], int correct_cipher_block[4][4], int row, int column, char wrong_cipher[35]){
    //byte_temp存放的是第十轮密钥块各个位置的所有可能值在计算中出现的次数，其中第三个维度是所有可能值0x00-0xff的集合
    int X, e, round, index;
    int wrong_cipher_block[4][4], coeff[4] = {2, 1, 1, 3};
    int correct_byte[4][256], correct_byte_num[4], cnt1;
    //correct_byte_num是correct_byte数组的计数器，correct_byte[4][256]中存放了注入错误能影响到的四个位置所有可能的X值
    bool round_check[4];
    str2block(&wrong_cipher_block, wrong_cipher);
    for (e = 0; e < 256; e++) { //遍历e
        memset(round_check, false, sizeof(bool) * 4);
        memset(correct_byte_num, 0, sizeof(int) * 4);
        for (round = 0; round < 4; round++) {
            for (X = 0; X < 256; X++) { //遍历X
                index = X ^ GF2_mul(e, coeff[(round + 4 - row) % 4], 0x11b);
                if (AES_S_box[index >> 4][index & 0xf] == (AES_S_box[X >> 4][X & 0xf] ^ wrong_cipher_block[round][(column + 4 - round) % 4] ^ correct_cipher_block[round][(column + 4 - round) % 4])) {
                    round_check[round] = true;
                    correct_byte[round][correct_byte_num[round]++] =
                            AES_S_box[X >> 4][X & 0xf] ^ correct_cipher_block[round][(column + 4 - round) % 4];
                }
            }
        }
        if (round_check[0] == true && round_check[1] == true && round_check[2] == true && round_check[3] == true)
            for (round = 0; round < 4; round++)
                for (cnt1 = 0; cnt1 < correct_byte_num[round]; cnt1++)
                    (*key10_temp)[round][(column + 4 - round) % 4][correct_byte[round][cnt1]]++;
    }
}

int check_byte(const int byte_temp[256]){
    int cnt, max = 0, pos;
    for(cnt = 0; cnt < 256; cnt++){
        if(max < byte_temp[cnt]){
            max = byte_temp[cnt];
            pos = cnt;
        }
    }
    for(cnt = 0; cnt < 256; cnt++)
        if(byte_temp[cnt] == max && cnt != pos)
            return -1;
    return pos;
}

void inv_Extend_key_128(int (*key_board)[4][44]){
    int row, column, word_temp[4];
    for(column = 39; column >=0; column--){
        if(column%4 != 0)
            for(row = 0; row < 4; row++)
                (*key_board)[row][column] = (*key_board)[row][column+4] ^ (*key_board)[row][column+3];
        else{
            for(row = 0; row < 4; row++){
                word_temp[row] = (*key_board)[(row+1)%4][column + 3];
                (*key_board)[row][column] = AES_S_box[word_temp[row] >> 4][word_temp[row] & 0xf];
            }
            (*key_board)[0][column] = (*key_board)[0][column] ^ Rcon[column/4];
            for(row = 0; row < 4; row++)
                (*key_board)[row][column] = (*key_board)[row][column+4] ^ (*key_board)[row][column];
        }
    }
}

/*
* 可选择normal和debug两种模式:
* 1:如果为normal模式，不进行如下的校验，不保证能够在数据很少的情况下得到正确结果;
* 2:如果为debug模式，将对结果进行校验。
*      *如果已经确定第十轮密钥块受影响4个位置的正确值，则跳出循环;
*      *如果已遍历所有错误密钥仍未得到唯一正确结果，提示错误
*/
void get_key_128(int (*key_board)[4][44], int correct_cipher_block[4][4], wc_inf wrong_cipher[4][4], char mode){
    int key10_temp[4][4][256] = {0};
    int r, c, cnt, round, flag, used_wc_num, correct;
    for(c = 0; c < 4; c++){
        used_wc_num = 0;
        for(r = 0; r < 4; r++){
            for(cnt = 0; cnt < wrong_cipher[r][c].num; cnt++){
                flag = 0;
                get_prob_byte(&key10_temp, correct_cipher_block, r, c, wrong_cipher[r][c].cipher[cnt]);
                used_wc_num++;
                if(mode == 'D'){
                    for(round = 0; round < 4; round++)
                        if((correct = check_byte(key10_temp[round][(c + 4 - round) % 4])) != -1){
                            (*key_board)[round][(c+4-round)%4+40] = correct;
                            flag++;
                        }
                    if(flag == 4){
                        printf("pos:(%d, %d)(%d, %d)(%d, %d)(%d, %d) is confirmed\nuse %d wrong cipher\n",
                               0,(c+4)%4, 1,(c+3)%4, 2,(c+2)%4, 3,(c+1)%4, used_wc_num);
                        goto pos;
                    }
                }
            }
        }
        pos:;
    }
    if(mode == 'N'){
        for(r = 0; r < 4; r++){
            for(c = 0; c < 4; c++){
                int appear_num = 0;
                for(cnt = 0; cnt < 4; cnt++)
                    appear_num += wrong_cipher[cnt][(r+c)%4].num;
                for(cnt = 0; cnt < 256; cnt++){
                    if(key10_temp[r][c][cnt] == appear_num)
                        (*key_board)[r][c+40] = cnt;
                }
            }
        }
    }
    inv_Extend_key_128(key_board);
}