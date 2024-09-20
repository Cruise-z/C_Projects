//
// Created by 赵睿智 on 2022/4/7.
//

#include "SM4_enc_dec.h"
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

ui SM4_S_box[16][16] = {0xD6, 0x90, 0xE9, 0xFE, 0xCC, 0xE1, 0x3D, 0xB7, 0x16, 0xB6, 0x14, 0xC2, 0x28, 0xFB, 0x2C, 0x05,
                        0x2B, 0x67, 0x9A, 0x76, 0x2A, 0xBE, 0x04, 0xC3, 0xAA, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
                        0x9C, 0x42, 0x50, 0xF4, 0x91, 0xEF, 0x98, 0x7A, 0x33, 0x54, 0x0B, 0x43, 0xED, 0xCF, 0xAC, 0x62,
                        0xE4, 0xB3, 0x1C, 0xA9, 0xC9, 0x08, 0xE8, 0x95, 0x80, 0xDF, 0x94, 0xFA, 0x75, 0x8F, 0x3F, 0xA6,
                        0x47, 0x07, 0xA7, 0xFC, 0xF3, 0x73, 0x17, 0xBA, 0x83, 0x59, 0x3C, 0x19, 0xE6, 0x85, 0x4F, 0xA8,
                        0x68, 0x6B, 0x81, 0xB2, 0x71, 0x64, 0xDA, 0x8B, 0xF8, 0xEB, 0x0F, 0x4B, 0x70, 0x56, 0x9D, 0x35,
                        0x1E, 0x24, 0x0E, 0x5E, 0x63, 0x58, 0xD1, 0xA2, 0x25, 0x22, 0x7C, 0x3B, 0x01, 0x21, 0x78, 0x87,
                        0xD4, 0x00, 0x46, 0x57, 0x9F, 0xD3, 0x27, 0x52, 0x4C, 0x36, 0x02, 0xE7, 0xA0, 0xC4, 0xC8, 0x9E,
                        0xEA, 0xBF, 0x8A, 0xD2, 0x40, 0xC7, 0x38, 0xB5, 0xA3, 0xF7, 0xF2, 0xCE, 0xF9, 0x61, 0x15, 0xA1,
                        0xE0, 0xAE, 0x5D, 0xA4, 0x9B, 0x34, 0x1A, 0x55, 0xAD, 0x93, 0x32, 0x30, 0xF5, 0x8C, 0xB1, 0xE3,
                        0x1D, 0xF6, 0xE2, 0x2E, 0x82, 0x66, 0xCA, 0x60, 0xC0, 0x29, 0x23, 0xAB, 0x0D, 0x53, 0x4E, 0x6F,
                        0xD5, 0xDB, 0x37, 0x45, 0xDE, 0xFD, 0x8E, 0x2F, 0x03, 0xFF, 0x6A, 0x72, 0x6D, 0x6C, 0x5B, 0x51,
                        0x8D, 0x1B, 0xAF, 0x92, 0xBB, 0xDD, 0xBC, 0x7F, 0x11, 0xD9, 0x5C, 0x41, 0x1F, 0x10, 0x5A, 0xD8,
                        0x0A, 0xC1, 0x31, 0x88, 0xA5, 0xCD, 0x7B, 0xBD, 0x2D, 0x74, 0xD0, 0x12, 0xB8, 0xE5, 0xB4, 0xB0,
                        0x89, 0x69, 0x97, 0x4A, 0x0C, 0x96, 0x77, 0x7E, 0x65, 0xB9, 0xF1, 0x09, 0xC5, 0x6E, 0xC6, 0x84,
                        0x18, 0xF0, 0x7D, 0xEC, 0x3A, 0xDC, 0x4D, 0x20, 0x79, 0xEE, 0x5F, 0x3E, 0xD7, 0xCB, 0x39, 0x48};

ui Fk[4] = {0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc};

ui Ck[32] = {0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
             0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
             0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
             0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
             0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
             0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
             0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
             0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279};

void str2group_block(ui (*group)[33][4], char *hex_32byte_str){
    hex_32byte_str += 2;
    int i;
    for(i = 0; i < 4; i++)
        (*group)[0][i] = hex2bin_index(hex_32byte_str[8*i+0])*0x10000000
                + hex2bin_index(hex_32byte_str[8*i+1])*0x1000000
                + hex2bin_index(hex_32byte_str[8*i+2])*0x100000
                + hex2bin_index(hex_32byte_str[8*i+3])*0x10000
                + hex2bin_index(hex_32byte_str[8*i+4])*0x1000
                + hex2bin_index(hex_32byte_str[8*i+5])*0x100
                + hex2bin_index(hex_32byte_str[8*i+6])*0x10
                + hex2bin_index(hex_32byte_str[8*i+7])*0x1;
}

void str2group(ui (*group)[4], char *hex_32byte_str){
    hex_32byte_str += 2;
    int i;
    for(i = 0; i < 4; i++)
        (*group)[i] = hex2bin_index(hex_32byte_str[8*i+0])*0x10000000
                + hex2bin_index(hex_32byte_str[8*i+1])*0x1000000
                + hex2bin_index(hex_32byte_str[8*i+2])*0x100000
                + hex2bin_index(hex_32byte_str[8*i+3])*0x10000
                + hex2bin_index(hex_32byte_str[8*i+4])*0x1000
                + hex2bin_index(hex_32byte_str[8*i+5])*0x100
                + hex2bin_index(hex_32byte_str[8*i+6])*0x10
                + hex2bin_index(hex_32byte_str[8*i+7])*0x1;
}

void SM4_str2key(ui (*Mk)[4], char *hex_32byte_str){
    hex_32byte_str += 2;
    int i;
    for(i = 0; i < 4; i++)
        (*Mk)[i] = hex2bin_index(hex_32byte_str[8*i+0])*0x10000000
                + hex2bin_index(hex_32byte_str[8*i+1])*0x1000000
                + hex2bin_index(hex_32byte_str[8*i+2])*0x100000
                + hex2bin_index(hex_32byte_str[8*i+3])*0x10000
                + hex2bin_index(hex_32byte_str[8*i+4])*0x1000
                + hex2bin_index(hex_32byte_str[8*i+5])*0x100
                + hex2bin_index(hex_32byte_str[8*i+6])*0x10
                + hex2bin_index(hex_32byte_str[8*i+7])*0x1;
}

ui r_sl(ui hex_8byte_num, int sl_bit){
    return (hex_8byte_num >> (32 - sl_bit)) | ((hex_8byte_num << sl_bit) & 0xffffffff);
}

ui nl_tran(ui hex_8byte_num){
    ui a0 = hex_8byte_num >> 24;
    ui a1 = (hex_8byte_num & 0x00ff0000) >> 16;
    ui a2 = (hex_8byte_num & 0x0000ff00) >> 8;
    ui a3 = (hex_8byte_num & 0x000000ff);
    ui ans = SM4_S_box[a0 >> 4][a0 & 0xf] * 0x1000000
             + SM4_S_box[a1>>4][a1&0xf] * 0x10000
             + SM4_S_box[a2>>4][a2&0xf] * 0x100
             + SM4_S_box[a3>>4][a3&0xf];
    return ans;
}

ui l_tran_cipher(ui hex_8byte_num){
    return hex_8byte_num ^ r_sl(hex_8byte_num, 2) ^ r_sl(hex_8byte_num, 10) ^ r_sl(hex_8byte_num, 18) ^ r_sl(hex_8byte_num, 24);
}

ui l_tran_key(ui hex_8byte_num){
    return hex_8byte_num ^ r_sl(hex_8byte_num, 13) ^ r_sl(hex_8byte_num, 23);
}

ui T_func(ui hex_8byte_num, char mode){
    if(mode == 'k')
        return l_tran_key(nl_tran(hex_8byte_num));
    else if(mode == 'c')
        return l_tran_cipher(nl_tran(hex_8byte_num));
    else
        return -1;
}

void extend_key(ui (*K)[36], const ui Mk[4]){
    int i;
    ui temp;
    for(i = 0; i < 4; i++)
        (*K)[i] = Mk[i] ^ Fk[i];
    for(; i < 36; i++){
        temp = (*K)[i-1] ^ (*K)[i-2] ^ (*K)[i-3] ^ (Ck)[i-4];
        temp = T_func(temp, 'k');
        (*K)[i] = temp ^ (*K)[i-4];
    }
}

void SM4(ui (*group)[33][4], const ui K[36], char mode){
    int cnt;
    ui temp;
    if(mode == 'e'){
        for(cnt = 1; cnt < 33; cnt++){
            (*group)[cnt][0] = (*group)[cnt-1][1];
            (*group)[cnt][1] = (*group)[cnt-1][2];
            (*group)[cnt][2] = (*group)[cnt-1][3];
            temp = (*group)[cnt-1][1] ^ (*group)[cnt-1][2] ^ (*group)[cnt-1][3] ^ K[cnt+3];
            temp = T_func(temp, 'c');
            (*group)[cnt][3] = temp ^ (*group)[cnt-1][0];
        }
    }else if(mode == 'd'){
        for(cnt = 1; cnt < 33; cnt++){
            (*group)[cnt][0] = (*group)[cnt - 1][1];
            (*group)[cnt][1] = (*group)[cnt - 1][2];
            (*group)[cnt][2] = (*group)[cnt - 1][3];
            temp = (*group)[cnt - 1][1] ^ (*group)[cnt - 1][2] ^ (*group)[cnt - 1][3] ^ K[36-cnt];
            temp = T_func(temp, 'c');
            (*group)[cnt][3] = temp ^ (*group)[cnt - 1][0];
        }
    }
    for(cnt = 0; cnt < 4; cnt++)
        (*group)[0][cnt] = (*group)[32][3-cnt];
}

void CTR_counter_add(ui (*counter)[4], ui add_num){
    int cnt0 = 3, cnt;
    for(cnt = 0; cnt < add_num; cnt++){
        while((*counter)[cnt0] == 0xffffffff){
            if(cnt0 == 0)
                break;
            (*counter)[cnt0]++;
            cnt0--;
        }
        (*counter)[cnt0]++;
    }
}

void OFB_lshift_fill(ui (*iv_temp)[4], int byte, const ui *fill_byte_array){
    ui temp[16];
    int cnt;
    for(cnt = 0; cnt < 4; cnt++){
        temp[4*cnt] = iv_temp[0][cnt] >> 24;
        temp[4*cnt+1] = (iv_temp[0][cnt]&0x00ff0000) >> 16;
        temp[4*cnt+2] = (iv_temp[0][cnt]&0x0000ff00) >> 8;
        temp[4*cnt+3] = (iv_temp[0][cnt]&0x000000ff);
    }
    for(cnt = 0; cnt < 16-byte; cnt++)
        temp[cnt] = temp[cnt+byte];
    for(cnt = 0; cnt < byte; cnt++)
        temp[cnt+16-byte] = fill_byte_array[cnt];
    for(cnt = 0; cnt < 4; cnt++){
        (*iv_temp)[cnt] = temp[4*cnt]*0x1000000
                + temp[4*cnt+1]*0x10000
                + temp[4*cnt+2]*0x100
                + temp[4*cnt+3];
    }
}

void output_group(ui group[33][4]){
    int cnt;
    printf("0x");
    for(cnt = 0; cnt < 4; cnt++)
        printf("%08x", group[0][cnt]);
}

void output_key_list(ui key_list[36]){
    int cnt;
    for(cnt = 0; cnt < 36; cnt++){
        printf("%08x\n", key_list[cnt]);
    }
}