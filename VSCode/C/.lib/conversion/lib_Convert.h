//
// Created by 赵睿智 on 2022/3/31.
//

#ifndef CRYPTO_EXPERIMENT_LIB_CONVERT_H
#define CRYPTO_EXPERIMENT_LIB_CONVERT_H

#include <gmp.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include "../My_BigNum_GF_cal/My_BigNum_GF_cal.h"

#define byte uint8_t
#define bit  bool

#define arrLEN 1<<20

#define ENDIAN_LITTLE  1234
#define ENDIAN_BIG     4321

#define ENDIANNESS ENDIAN_LITTLE

/* Swap bytes in 16 bit value. */
#define __bswap_16(x) \
     ((unsigned short int)     \
      ((((x) >> 8) & 0xff)     \
     | (((x) & 0xff) << 8)))

/* Swap bytes in 32 bit value. */
#define __bswap_32(x)    \
     ((((x) & 0xff000000) >> 24)  \
     | (((x) & 0x00ff0000) >>  8) \
     | (((x) & 0x0000ff00) <<  8) \
     | (((x) & 0x000000ff) << 24))

/* Swap bytes in 64 bit value. */
#define __bswap_64(x)               \
     ((((x) & 0xff00000000000000ull) >> 56)  \
     | (((x) & 0x00ff000000000000ull) >> 40) \
     | (((x) & 0x0000ff0000000000ull) >> 24) \
     | (((x) & 0x000000ff00000000ull) >>  8) \
     | (((x) & 0x00000000ff000000ull) <<  8) \
     | (((x) & 0x0000000000ff0000ull) << 24) \
     | (((x) & 0x000000000000ff00ull) << 40) \
     | (((x) & 0x00000000000000ffull) << 56))

#if (ENDIANNESS == ENDIAN_LITTLE)
#define htole16(x)      (x)
#define htole32(x)      (x)
#define htole64(x)      (x)

#define htobe16(x)      __bswap_16(x)
#define htobe32(x)      __bswap_32(x)
#define htobe64(x)      __bswap_64(x)
#else
#define htole16(x)      __bswap_16(x)
#define htole32(x)      __bswap_32(x)
#define htole64(x)      __bswap_64(x)

#define htobe16(x)      (x)
#define htobe32(x)      (x)
#define htobe64(x)      (x)
#endif

#define le16toh(x)      htole16(x)
#define le32toh(x)      htole32(x)
#define le64toh(x)      htole64(x)

#define be16toh(x)      htobe16(x)
#define be32toh(x)      htobe32(x)
#define be64toh(x)      htobe64(x)

typedef struct byte_array{
    byte arr[arrLEN];
    uint64_t len;
    //初始化byte_array数组只需要初始化len的长度即可
}byte_arr, *byte_ptr;

typedef struct bit_array{
    bit arr[arrLEN];
    uint64_t len;
    //初始化bit_array数组只需要初始化len的长度即可
}bit_arr, *bit_ptr;

extern char hex2bin_table[16][5];
extern char bin2hex_table[16];
extern char oct2hex_table[16];

int hex2bin_index(char hex);
void hex2bin(char *ans_str, char *hex_str);
void bin2hex(char *ans_str, char *bin_str);
int hex2oct_index(char hex);

void init_byte_arr(byte_ptr src);
size_t byte_len(byte_arr arr);
size_t bit_len(bit_arr arr);
void byte_cpy(byte_ptr dest, byte_arr src, uint64_t len);
void bit_copy(bit_ptr dest, bit_arr src, uint64_t src_start, uint64_t len);
void byte_cat(byte_ptr src1_ptr, byte_arr src2);
void bit_cat(bit_ptr src1_ptr, bit_arr src2);
void byteStr2num(mpz_t *ans, byte_arr arr);
void bitStr2num(mpz_t *ans, bit_arr arr);
void str2byteString(byte_ptr ANS_ptr, char *hex_src, uint64_t formatLen);
void mpzNum2byteString(byte_ptr ANS_ptr, mpz_t src, uint64_t formatLen);
void str2bitString(bit_ptr ANS_ptr, char *bit_src, uint64_t formatLen);
void bit2byte(byte_ptr ans, bit_arr src, int formatLen);
void byte2bitString(bit_ptr ans, byte_arr src, uint64_t formatLen);

void printBit(bit_arr arr);
void printHex(byte_arr arr);

int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput, int outSize);
void WStr2ByteStr_utf8(byte_ptr ans, wchar_t *str);



#endif //CRYPTO_EXPERIMENT_LIB_CONVERT_H
