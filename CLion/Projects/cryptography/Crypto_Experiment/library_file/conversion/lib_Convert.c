//
// Created by 赵睿智 on 2022/3/31.
//

#include "lib_Convert.h"
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

char hex2bin_table[16][5] = {"0000\0", "0001\0", "0010\0", "0011\0",
                             "0100\0", "0101\0", "0110\0", "0111\0",
                             "1000\0", "1001\0", "1010\0", "1011\0",
                             "1100\0", "1101\0", "1110\0", "1111\0" };

char bin2hex_table[16] = {"0123456789abcdef"};

char oct2hex_table[16] = {'0', '1', '2', '3',
                          '4', '5', '6', '7',
                          '8', '9', 'a', 'b',
                          'c', 'd', 'e', 'f'};

int hex2oct_index(char hex){
    if(hex>='0' && hex<='9')
        return hex-'0';
    else if(hex>='A' && hex<='F')
        return hex-'A'+10;
    else if(hex>='a' && hex<='f')
        return hex-'a'+10;
    else
        return -1;
}

int hex2bin_index(char hex){
    if(hex>='0' && hex<='9')
        return hex-'0';
    else if(hex>='A' && hex<='F')
        return hex-'A'+10;
    else if(hex>='a' && hex<='f')
        return hex-'a'+10;
    else
        return -1;
}

void hex2bin(char *ans_str, char *hex_str){
    int len = (int)strlen(hex_str), i;
    ans_str[0] = '\0';
    for(i = 0; i < len; i++)
        strcat(ans_str, hex2bin_table[hex2bin_index(hex_str[i])]);
}

void bin2hex(char *ans_str, char *bin_str){
    int len = (int)strlen(bin_str), round = len/4, round_cnt, cnt, sum;
    for(round_cnt = 0; round_cnt < round; round_cnt++){
        sum = 0;
        for(cnt = 0; cnt < 4; cnt++){
            sum *= 2;
            sum += bin_str[round_cnt*4+cnt]-'0';
        }
        ans_str[round_cnt] = bin2hex_table[sum];
    }
    ans_str[round_cnt] = '\0';
}

void init_byte_arr(byte_ptr src){
    src->len = 0;
}

size_t byte_len(byte_arr arr){
    return arr.len;
}

size_t bit_len(bit_arr arr){
    return arr.len;
}

void byte_cpy(byte_ptr dest, byte_arr src, uint64_t len){
    int64_t cnt;
    for(cnt = 0; cnt < (src.len>len?len:src.len); cnt++)
        dest->arr[cnt] = src.arr[cnt];
    dest->len = (src.len>len?len:src.len);
}

void bit_copy(bit_ptr dest, bit_arr src, uint64_t src_start, uint64_t len){
    //start指的是需要复制的比特串数组的起始位置
    uint64_t cnt, Len = (len+src_start)>src.len?(src.len-src_start):len;
    for(cnt = 0; cnt < Len; cnt++)
        dest->arr[cnt] = src.arr[cnt+src_start];
    dest->len = Len;
}

void byte_cat(byte_ptr src1_ptr, byte_arr src2){
    int64_t cnt;
    for(cnt = 0; cnt < src2.len; cnt++)
        src1_ptr->arr[src1_ptr->len+cnt] = src2.arr[cnt];
    src1_ptr->len += src2.len;
}

void bit_cat(bit_ptr src1_ptr, bit_arr src2){
    int64_t cnt;
    for(cnt = 0; cnt < src2.len; cnt++)
        src1_ptr->arr[src1_ptr->len+cnt] = src2.arr[cnt];
    src1_ptr->len += src2.len;
}

void byteStr2num(mpz_t *ans, byte_arr arr){
    mpz_init_set_ui(*ans, 0);
    int cnt;
    for(cnt = 0; cnt < arr.len; cnt++){
        mpz_mul_ui(*ans, *ans, 256);
        mpz_add_ui(*ans, *ans, arr.arr[cnt]);
    }
}

void bitStr2num(mpz_t *ans, bit_arr arr){
    mpz_init_set_ui(*ans, 0);
    int cnt;
    for(cnt = 0; cnt < arr.len; cnt++){
        mpz_mul_ui(*ans, *ans, 2);
        mpz_add_ui(*ans, *ans, arr.arr[cnt]);
    }
}

void str2byteString(byte_ptr ANS_ptr, char *hex_src, uint64_t formatLen){
    //formatLen为0默认不含前缀0字节进行标准输出
    unsigned int len = strlen(hex_src), cnt;
    char temp[LEN];
    if(len % 2 != 0){
        strcpy(temp, "0\0");
        strcat(temp, hex_src);
        len = strlen(temp);
    }else
        strcpy(temp, hex_src);
    if(formatLen <= len/2){
        for(cnt = 0; cnt < len/2; cnt++)
            ANS_ptr->arr[cnt] =
                    (hex2oct_index(temp[2*cnt]))*0x10 + hex2oct_index(temp[2*cnt+1]);
        ANS_ptr->len = len/2;
    }else{
        for(cnt = 0; cnt < formatLen-len/2; cnt++)
            ANS_ptr->arr[cnt] = 0x00;
        for(cnt = 0; cnt < len/2; cnt++)
            ANS_ptr->arr[cnt+formatLen-len/2] =
                    (hex2oct_index(temp[2*cnt]))*0x10 + hex2oct_index(temp[2*cnt+1]);
        ANS_ptr->len = formatLen;
    }
}

void mpzNum2byteString(byte_ptr ANS_ptr, mpz_t src, uint64_t formatLen){
    char *str_src = NULL;
    str_src = mpz_get_str(str_src, 16, src);
    str2byteString(ANS_ptr, str_src, formatLen);
}

void str2bitString(bit_ptr ANS_ptr, char *bit_src, uint64_t formatLen){
    //formatLen为0默认不含前缀0比特进行标准输出
    uint64_t len = strlen(bit_src), cnt;
    ANS_ptr->len = 0;
    if(formatLen > len){
        for(cnt = 0; cnt < formatLen-len; cnt++)
            ANS_ptr->arr[cnt] = 0;
        ANS_ptr->len = cnt;
    }
    for(cnt = 0; cnt < strlen(bit_src); cnt++)
        ANS_ptr->arr[ANS_ptr->len+cnt] = bit_src[cnt] - '0';
    ANS_ptr->len += cnt;
}

void bit2byte(byte_ptr ans, bit_arr src, int formatLen){
    //formatLen为0默认不含前缀0字节进行标准输出
    ans->len = 0;
    uint64_t cnt;
    bit_arr temp;
    temp.len = 0;
    if(src.len%8 != 0){
        for(cnt = 0; cnt < 8-src.len%8; cnt++)
            temp.arr[temp.len++] = 0;
        bit_cat(&temp, src);
    }else
        bit_cat(&temp, src);
    if(formatLen > temp.len/8){
        for(cnt = 0; cnt < formatLen-temp.len/8; cnt++)
            ans->arr[cnt] = 0x00;
        ans->len = cnt;
    }
    byte_arr temp2;
    for(temp2.len = 0; temp2.len < temp.len/8; temp2.len++){
        temp2.arr[temp2.len] = 0;
        for(cnt = 0; cnt < 8; cnt++){
            temp2.arr[temp2.len] *= 2;
            temp2.arr[temp2.len] += temp.arr[temp2.len*8+cnt];
        }
    }
    byte_cat(ans, temp2);
}

void byte2bitString(bit_ptr ans, byte_arr src, uint64_t formatLen){
    //formatLen为0默认不含前缀0比特进行标准输出
    ans->len = 0;
    char *bin_str = NULL;
    mpz_t temp;
    byteStr2num(&temp, src);
    bin_str = mpz_get_str(bin_str, 2, temp);
    int64_t len = strlen(bin_str), cnt;
    if(len >= formatLen)
        str2bitString(ans, bin_str, 0);
    else{
        char str[arrLEN];
        for(cnt = 0; cnt < formatLen-len; cnt++)
            str[cnt] = '0';
        str[cnt] = '\0';
        strcat(str, bin_str);
        str2bitString(ans, str, formatLen);
    }
}

void printBit(bit_arr arr){
    uint64_t cnt;
    for(cnt = 0; cnt < arr.len; cnt++){
        printf("%d", arr.arr[cnt]);
    }
    printf("\n");
}

void printHex(byte_arr arr)
{
    for (int i = 0; i < arr.len; ++i)
        printf("%02x", arr.arr[i]);
    printf("\n");
}

/*****************************************************************************
 * 将一个字符的Unicode(UCS-2和UCS-4)编码转换成UTF-8编码.
 *
 * 参数:
 *    unic     字符的Unicode编码值
 *    pOutput  指向输出的用于存储UTF8编码值的缓冲区的指针
 *    outsize  pOutput缓冲的大小
 *
 * 返回值:
 *    返回转换后的字符的UTF8编码所占的字节数, 如果出错则返回 0 .
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 *     2. 请保证 pOutput 缓冲区有最少有 6 字节的空间大小!
 ****************************************************************************/
int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput, int outSize)
{
    assert(pOutput != NULL);
    assert(outSize >= 6);

    if ( unic <= 0x0000007F )
    {
        // * U-00000000 - U-0000007F:  0xxxxxxx
        *pOutput     = (unic & 0x7F);
        return 1;
    }
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )
    {
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        *(pOutput+1) = (unic & 0x3F) | 0x80;
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;
        return 2;
    }
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )
    {
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *(pOutput+2) = (unic & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;
        return 3;
    }
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )
    {
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+3) = (unic & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;
        return 4;
    }
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )
    {
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+4) = (unic & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;
        return 5;
    }
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )
    {
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+5) = (unic & 0x3F) | 0x80;
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;
        return 6;
    }

    return 0;
}
// #c---end

void WStr2ByteStr_utf8(byte_ptr ans, wchar_t *str){
    byte_arr temp;
    ans->len = 0;
    for(int cnt = 0; cnt < wcslen(str); cnt++){
        temp.len = enc_unicode_to_utf8_one(str[cnt], temp.arr, LEN);
        byte_cat(ans, temp);
    }
}