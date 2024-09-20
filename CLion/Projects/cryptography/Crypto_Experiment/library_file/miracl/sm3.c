/* ====================================================================
 * Copyright (c) 2014 - 2017 The GmSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the GmSSL Project.
 *    (http://gmssl.org/)"
 *
 * 4. The name "GmSSL Project" must not be used to endorse or promote
 *    products derived from this software without prior written
 *    permission. For written permission, please contact
 *    guanzhi1980@gmail.com.
 *
 * 5. Products derived from this software may not be called "GmSSL"
 *    nor may "GmSSL" appear in their names without prior written
 *    permission of the GmSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the GmSSL Project
 *    (http://gmssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE GmSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE GmSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */

#include <string.h>
#include "sm3.h"

#ifdef CPU_BIGENDIAN

#define cpu_to_be16(v) (v)
#define cpu_to_be32(v) (v)
#define be16_to_cpu(v) (v)
#define be32_to_cpu(v) (v)

#else

#define cpu_to_le16(v) (v)
#define cpu_to_le32(v) (v)
#define le16_to_cpu(v) (v)
#define le32_to_cpu(v) (v)

#define cpu_to_be16(v) (((v)<< 8) | ((v)>>8))
#define cpu_to_be32(v) (((v)>>24) | (((v)>>8)&0xff00) | (((v)<<8)&0xff0000) | ((v)<<24))
#define be16_to_cpu(v) cpu_to_be16(v)
#define be32_to_cpu(v) cpu_to_be32(v)

#endif

#define ROTATELEFT(X,n)  (((X)<<(n)) | ((X)>>(32-(n))))

#define P0(x) ((x) ^  ROTATELEFT((x),9)  ^ ROTATELEFT((x),17))
#define P1(x) ((x) ^  ROTATELEFT((x),15) ^ ROTATELEFT((x),23))

#define FF0(x,y,z) ( (x) ^ (y) ^ (z))
#define FF1(x,y,z) (((x) & (y)) | ( (x) & (z)) | ( (y) & (z)))

#define GG0(x,y,z) ( (x) ^ (y) ^ (z))
#define GG1(x,y,z) (((x) & (y)) | ( (~(x)) & (z)) )


static void sm3_compress(unsigned int digest[8], const unsigned char block[64])
{
	int j;
	unsigned int W[68], W1[64];
	const unsigned int *pblock = (const unsigned int *)block;

	unsigned int A = digest[0];
	unsigned int B = digest[1];
	unsigned int C = digest[2];
	unsigned int D = digest[3];
	unsigned int E = digest[4];
	unsigned int F = digest[5];
	unsigned int G = digest[6];
	unsigned int H = digest[7];
	unsigned int SS1,SS2,TT1,TT2,T[64];

	for (j = 0; j < 16; j++) {
		W[j] = cpu_to_be32(pblock[j]);
	}
	for (j = 16; j < 68; j++) {
		W[j] = P1( W[j-16] ^ W[j-9] ^ ROTATELEFT(W[j-3],15)) ^ ROTATELEFT(W[j - 13],7 ) ^ W[j-6];;
	}
	for( j = 0; j < 64; j++) {
		W1[j] = W[j] ^ W[j+4];
	}

	for(j =0; j < 16; j++) {

		T[j] = 0x79CC4519;
		SS1 = ROTATELEFT((ROTATELEFT(A,12) + E + ROTATELEFT(T[j],j)), 7);
		SS2 = SS1 ^ ROTATELEFT(A,12);
		TT1 = FF0(A,B,C) + D + SS2 + W1[j];
		TT2 = GG0(E,F,G) + H + SS1 + W[j];
		D = C;
		C = ROTATELEFT(B,9);
		B = A;
		A = TT1;
		H = G;
		G = ROTATELEFT(F,19);
		F = E;
		E = P0(TT2);
	}

	for(j =16; j < 64; j++) {

		T[j] = 0x7A879D8A;
		SS1 = ROTATELEFT((ROTATELEFT(A,12) + E + ROTATELEFT(T[j],j%32)), 7);
		SS2 = SS1 ^ ROTATELEFT(A,12);
		TT1 = FF1(A,B,C) + D + SS2 + W1[j];
		TT2 = GG1(E,F,G) + H + SS1 + W[j];
		D = C;
		C = ROTATELEFT(B,9);
		B = A;
		A = TT1;
		H = G;
		G = ROTATELEFT(F,19);
		F = E;
		E = P0(TT2);
	}

	digest[0] ^= A;
	digest[1] ^= B;
	digest[2] ^= C;
	digest[3] ^= D;
	digest[4] ^= E;
	digest[5] ^= F;
	digest[6] ^= G;
	digest[7] ^= H;
}

void sm3_init(sm3_ctx_t *ctx)
{
	ctx->digest[0] = 0x7380166F;
	ctx->digest[1] = 0x4914B2B9;
	ctx->digest[2] = 0x172442D7;
	ctx->digest[3] = 0xDA8A0600;
	ctx->digest[4] = 0xA96F30BC;
	ctx->digest[5] = 0x163138AA;
	ctx->digest[6] = 0xE38DEE4D;
	ctx->digest[7] = 0xB0FB0E4E;

	ctx->nblocks = 0;
	ctx->num = 0;
}

void sm3_update(sm3_ctx_t *ctx, const unsigned char* data, size_t data_len)
{
	if (ctx->num) {
		unsigned int left = SM3_BLOCK_SIZE - ctx->num;
		if (data_len < left) {
			memcpy(ctx->block + ctx->num, data, data_len);
			ctx->num += data_len;
			return;
		} else {
			memcpy(ctx->block + ctx->num, data, left);
			sm3_compress(ctx->digest, ctx->block);
			ctx->nblocks++;
			data += left;
			data_len -= left;
		}
	}
	while (data_len >= SM3_BLOCK_SIZE) {
		sm3_compress(ctx->digest, data);
		ctx->nblocks++;
		data += SM3_BLOCK_SIZE;
		data_len -= SM3_BLOCK_SIZE;
	}
	ctx->num = data_len;
	if (data_len) {
		memcpy(ctx->block, data, data_len);
	}
}

void sm3_final(sm3_ctx_t *ctx, unsigned char *digest)
{
	int i;
	unsigned int *pdigest = (unsigned int *)digest;
	unsigned int *count = (unsigned int *)(ctx->block + SM3_BLOCK_SIZE - 8);

	ctx->block[ctx->num] = 0x80;

	if (ctx->num + 9 <= SM3_BLOCK_SIZE) {
		memset(ctx->block + ctx->num + 1, 0, SM3_BLOCK_SIZE - ctx->num - 9);
	} else {
		memset(ctx->block + ctx->num + 1, 0, SM3_BLOCK_SIZE - ctx->num - 1);
		sm3_compress(ctx->digest, ctx->block);
		memset(ctx->block, 0, SM3_BLOCK_SIZE - 8);
	}

	count[0] = cpu_to_be32((ctx->nblocks) >> 23);
	count[1] = cpu_to_be32((ctx->nblocks << 9) + (ctx->num << 3));

	sm3_compress(ctx->digest, ctx->block);
	for (i = 0; i < sizeof(ctx->digest)/sizeof(ctx->digest[0]); i++) {
		pdigest[i] = cpu_to_be32(ctx->digest[i]);
	}
}

void sm3(const unsigned char *msg, size_t msglen,
	unsigned char dgst[SM3_DIGEST_LENGTH])
{
	sm3_ctx_t ctx;

	sm3_init(&ctx);
	sm3_update(&ctx, msg, msglen);
	sm3_final(&ctx, dgst);

	memset(&ctx, 0, sizeof(sm3_ctx_t));
}

/**
 * HMAC_k(m) = H((k ^ opad), H((k ^ ipad), m))
 * pseudo-code:
 * function hmac(key, message)
 *	opad = [0x5c * blocksize]
 *	ipad = [0x36 * blocksize]
 *	if (length(key) > blocksize) then
 *		key = hash(key)
 *	end if
 *	for i from 0 to length(key) - 1 step 1
 *		ipad[i] = ipad[i] XOR key[i]
 *		opad[i] = opad[i] XOR key[i]
 *	end for
 *	return hash(opad || hash(ipad || message))
 * end function
 */


#define IPAD	0x36
#define OPAD	0x5C

void sm3_hmac_init(sm3_hmac_ctx_t *ctx, const unsigned char *key, size_t key_len)
{
	int i;

	if (key_len <= SM3_BLOCK_SIZE) {
		memcpy(ctx->key, key, key_len);
		memset(ctx->key + key_len, 0, SM3_BLOCK_SIZE - key_len);
	} else {
		sm3_init(&ctx->sm3_ctx);
		sm3_update(&ctx->sm3_ctx, key, key_len);
		sm3_final(&ctx->sm3_ctx, ctx->key);
		memset(ctx->key + SM3_DIGEST_LENGTH, 0,
			SM3_BLOCK_SIZE - SM3_DIGEST_LENGTH);
	}
	for (i = 0; i < SM3_BLOCK_SIZE; i++) {
		ctx->key[i] ^= IPAD;
	}

	sm3_init(&ctx->sm3_ctx);
	sm3_update(&ctx->sm3_ctx, ctx->key, SM3_BLOCK_SIZE);
}

void sm3_hmac_update(sm3_hmac_ctx_t *ctx,
	const unsigned char *data, size_t data_len)
{
	sm3_update(&ctx->sm3_ctx, data, data_len);
}

void sm3_hmac_final(sm3_hmac_ctx_t *ctx, unsigned char mac[SM3_HMAC_SIZE])
{
	int i;
	for (i = 0; i < SM3_BLOCK_SIZE; i++) {
		ctx->key[i] ^= (IPAD ^ OPAD);
	}
	sm3_final(&ctx->sm3_ctx, mac);
	sm3_init(&ctx->sm3_ctx);
	sm3_update(&ctx->sm3_ctx, ctx->key, SM3_BLOCK_SIZE);
	sm3_update(&ctx->sm3_ctx, mac, SM3_DIGEST_LENGTH);
	sm3_final(&ctx->sm3_ctx, mac);
}

void sm3_hmac(const unsigned char *data, size_t data_len,
	const unsigned char *key, size_t key_len,
	unsigned char mac[SM3_HMAC_SIZE])
{
	sm3_hmac_ctx_t ctx;
	sm3_hmac_init(&ctx, key, key_len);
	sm3_hmac_update(&ctx, data, data_len);
	sm3_hmac_final(&ctx, mac);
	memset(&ctx, 0, sizeof(ctx));
}

