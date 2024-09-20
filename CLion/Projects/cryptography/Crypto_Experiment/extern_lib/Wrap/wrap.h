//
// Created by 赵睿智 on 2022/5/18.
//

#ifndef CRYPTO_EXPERIMENT_WRAP_H
#define CRYPTO_EXPERIMENT_WRAP_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#define UTILS_ERR   (-1)
#define UTILS_OK    0
#define max_len     1ull<<15

extern int32_t utils_hex_string_2_bytes(const char *hextring, uint8_t *bytes, uint16_t *length);

extern int32_t utils_bytes_2_hex_string(const uint8_t *bytes, uint16_t length, char *hextring);

#ifdef __cplusplus
}
#endif

#endif //CRYPTO_EXPERIMENT_WRAP_H
