//
// Created by 赵睿智 on 2022/3/27.
//

#ifndef CRYPTO_EXPERIMENT_DES_ENC_DEC_H
#define CRYPTO_EXPERIMENT_DES_ENC_DEC_H

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>
#include "../My_Matrix_cal/My_Matrix_cal.h"
#include "../My_BigNum_GF_cal/My_BigNum_GF_cal.h"

#define DES_BLOCK_LEN 		8       // 8bytes

#define DES_KEY_LEN_8		8		// key-length: 8bytes  -> single DES
#define DES_KEY_LEN_16		16      // key-length: 16bytes -> triple DES (key1=key[0], key2=key[8], key3=key[0])
#define DES_KEY_LEN_24		24		// key-length: 24bytes -> triple DES (key1=key[0], key2=key[8], key3=key[16])

typedef enum des_mode_e {
    DES_MODE_ECB,
    DES_MODE_CBC
} des_mode_e;

int32_t crypto_des_encrypt(const uint8_t *data, uint32_t data_len, uint8_t *out, const uint8_t *iv,
                           const uint8_t *key, uint32_t key_len, des_mode_e mode);
int32_t crypto_des_decrypt(const uint8_t *data, uint32_t data_len, uint8_t *out, const uint8_t *iv,
                           const uint8_t *key, uint32_t key_len, des_mode_e mode);

#endif //CRYPTO_EXPERIMENT_DES_ENC_DEC_H
