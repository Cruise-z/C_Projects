//
// Created by 赵睿智 on 2022/5/19.
//

#ifndef CRYPTO_EXPERIMENT_HASH_H
#define CRYPTO_EXPERIMENT_HASH_H

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdint.h>
#include <locale.h>
#include "../conversion/lib_Convert.h"

///SHA-1 START
#define SHA1_DIGEST_LEN 20         	// SHA1 outputs a 20 byte digest

typedef struct sha1_ctx_t {
    uint32_t total[2];     	/*!< number of bytes processed  */
    uint32_t state[5];   	/*!< intermediate digest state  */
    uint8_t buffer[64];   	/*!< data block being processed */
} sha1_ctx_t;

void crypto_sha1_init(sha1_ctx_t *ctx);
void crypto_sha1_update(sha1_ctx_t *ctx, const uint8_t *input, uint32_t len);
void crypto_sha1_final(sha1_ctx_t *ctx, uint8_t *digest);

void get_wchar(FILE *des, wchar_t *wstr);

void crypto_SHA1(byte_ptr ans, byte_arr src);
void crypto_HMAC_SHA1(byte_ptr ans, byte_arr key, byte_arr m);
///SHA-1 END

///SM3 START
#define ERR_OK           0
#define ERR_ERR         (-1)  /* generic error */
#define ERR_INV_PARAM   (-2)  /* invalid parameter */
#define ERR_TOO_LONG    (-3)  /* too long */
#define ERR_STATE_ERR   (-4)  /* state error */

#ifdef DEBUG
#define DBG(...) printf(__VA_ARGS__)
#define DUMP_SCHED_DATA 1
#define DUMP_BLOCK_DATA 1
#define DUMP_BLOCK_HASH 1
#define DUMP_ROUND_DATA 1
#else
#define DBG(...)
#define DUMP_SCHED_DATA 0
#define DUMP_BLOCK_DATA 0
#define DUMP_BLOCK_HASH 0
#define DUMP_ROUND_DATA 0
#endif

#define HASH_BLOCK_SIZE		    64	/* 512 bits = 64 Bytes */
#define HASH_LEN_SIZE	 	    8	/* 64 bits = 8 bytes */
#define HASH_LEN_OFFSET         (HASH_BLOCK_SIZE - HASH_LEN_SIZE)

#define HASH_DIGEST_SIZE        32 /* 256 bits = 32 bytes */

#define HASH_PADDING_PATTERN 	0x80
#define HASH_ROUND_NUM			64

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef struct sm3_context {
    /* message total length in bytes */
    uint64_t total;

    /* intermedia hash value for each block */
    struct {
        uint32_t a;
        uint32_t b;
        uint32_t c;
        uint32_t d;
        uint32_t e;
        uint32_t f;
        uint32_t g;
        uint32_t h;
    }hash;

    /* last block */
    struct {
        uint32_t used;     /* used bytes */
        uint8_t  buf[64];  /* block data buffer */
    }last;
}SM3_CTX;

/* https://www.openssl.org/docs/man1.1.1/man3/SHA256_Final.html */

int SM3_Init(SM3_CTX *c);
int SM3_Update(SM3_CTX *c, const void *data, size_t len);
int SM3_Final(unsigned char *md, SM3_CTX *c);
byte *SM3(const unsigned char *d, size_t n, unsigned char *md);

///SM3 END


#endif //CRYPTO_EXPERIMENT_HASH_H
