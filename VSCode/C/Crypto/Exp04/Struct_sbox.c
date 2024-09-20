//
// Created by 赵睿智 on 2022/3/31.
//

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../../.lib/My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../../.lib/My_Matrix_cal/My_Matrix_cal.h"
#include "../../.lib/Classical_Cipher/Classical_Cipher.h"
#include "../../.lib/AES/AES_enc_dec.h"
#include "../../.lib/conversion/lib_Convert.h"

char s_box[16][16][3];
char inv_s_box[16][16][3];

char c[8] = "01100011";
char *mod_str = "11b";


void output_s_box(char sBox[16][16][3]);

int main(){
    int i, j, cnt;
    //初始化
    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            s_box[i][j][0] = oct2hex_table[i];
            s_box[i][j][1] = oct2hex_table[j];
            s_box[i][j][2] = '\0';
        }
    }
    output_s_box(s_box);
    //逆元代替
    mpz_gcd_re re;
    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            re = GF_2_hex_exEuclid(s_box[i][j], mod_str, mod_str);
            if(strlen(re.x_str) == 1){
                s_box[i][j][0] = '0';
                strcpy(&s_box[i][j][1], re.x_str);
            }else
                strcpy(s_box[i][j], re.x_str);
        }
    }
    output_s_box(s_box);
    //对字节的每位用0x63变换
    char bin_temp[9], xor_re[9];
    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            hex2bin(bin_temp, s_box[i][j]);
            for(cnt = 0; cnt < 8; cnt++){
                xor_re[cnt] = xor(bin_temp[cnt], bin_temp[(cnt+4)%8]);
                xor_re[cnt] = xor(xor_re[cnt], bin_temp[(cnt+3)%8]);
                xor_re[cnt] = xor(xor_re[cnt], bin_temp[(cnt+2)%8]);
                xor_re[cnt] = xor(xor_re[cnt], bin_temp[(cnt+1)%8]);
                xor_re[cnt] = xor(xor_re[cnt], c[cnt]);
            }
            xor_re[8] = '\0';
            bin2hex(s_box[i][j], xor_re);
        }
    }
    output_s_box(s_box);
    //输出逆矩阵
    char locate_pos[3];
    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            strcpy(locate_pos, s_box[i][j]);
            int row = hex2oct_index(locate_pos[0]), column = hex2oct_index(locate_pos[1]);
            inv_s_box[row][column][0] = oct2hex_table[i];
            inv_s_box[row][column][1] = oct2hex_table[j];
            inv_s_box[row][column][2] = '\0';
        }
    }
    output_s_box(inv_s_box);
}

void output_s_box(char sBox[16][16][3]){
    int i, j;
    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            printf("0x%s,", sBox[i][j]);
        }
        printf("\n");
    }
}