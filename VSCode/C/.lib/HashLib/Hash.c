//
// Created by 赵睿智 on 2022/5/19.
//

#include "Hash.h"

///SHA-1 START
/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n, b, i)                                              \
    {                                                                       \
        (n) = ((uint32_t)(b)[(i)] << 24) | ((uint32_t)(b)[(i) + 1] << 16) | \
              ((uint32_t)(b)[(i) + 2] << 8) | ((uint32_t)(b)[(i) + 3]);     \
    }
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n, b, i)                  \
    {                                           \
        (b)[(i)]     = (uint8_t)((n) >> 24);    \
        (b)[(i) + 1] = (uint8_t)((n) >> 16);    \
        (b)[(i) + 2] = (uint8_t)((n) >> 8);     \
        (b)[(i) + 3] = (uint8_t)((n));          \
    }
#endif

static const uint8_t sha1_padding[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static void local_sha1_process(sha1_ctx_t *ctx, const uint8_t data[64])
{
    uint32_t temp, W[16], A, B, C, D, E;

    GET_UINT32_BE(W[0], data, 0)
    GET_UINT32_BE(W[1], data, 4)
    GET_UINT32_BE(W[2], data, 8)
    GET_UINT32_BE(W[3], data, 12)
    GET_UINT32_BE(W[4], data, 16)
    GET_UINT32_BE(W[5], data, 20)
    GET_UINT32_BE(W[6], data, 24)
    GET_UINT32_BE(W[7], data, 28)
    GET_UINT32_BE(W[8], data, 32)
    GET_UINT32_BE(W[9], data, 36)
    GET_UINT32_BE(W[10], data, 40)
    GET_UINT32_BE(W[11], data, 44)
    GET_UINT32_BE(W[12], data, 48)
    GET_UINT32_BE(W[13], data, 52)
    GET_UINT32_BE(W[14], data, 56)
    GET_UINT32_BE(W[15], data, 60)

#define S(x, n) (((x) << (n)) | (((x) & 0xFFFFFFFF) >> (32 - (n))))

#define R(t)                                                             \
    (temp = W[((t) - 3) & 0x0F] ^ W[((t) - 8) & 0x0F] ^ W[((t) - 14) & 0x0F] ^ \
            W[(t) & 0x0F],                                                 \
     (W[(t) & 0x0F] = S(temp, 1)))

#define P(a, b, c, d, e, x)                \
    {                                      \
        (e) += S(a, 5) + F(b, c, d) + K + (x); \
        (b) = S(b, 30);                      \
    }

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];
    E = ctx->state[4];

#define F(x, y, z) (z ^ (x & (y ^ z)))
#define K 0x5A827999

    P(A, B, C, D, E, W[0])
    P(E, A, B, C, D, W[1])
    P(D, E, A, B, C, W[2])
    P(C, D, E, A, B, W[3])
    P(B, C, D, E, A, W[4])
    P(A, B, C, D, E, W[5])
    P(E, A, B, C, D, W[6])
    P(D, E, A, B, C, W[7])
    P(C, D, E, A, B, W[8])
    P(B, C, D, E, A, W[9])
    P(A, B, C, D, E, W[10])
    P(E, A, B, C, D, W[11])
    P(D, E, A, B, C, W[12])
    P(C, D, E, A, B, W[13])
    P(B, C, D, E, A, W[14])
    P(A, B, C, D, E, W[15])
    P(E, A, B, C, D, R(16))
    P(D, E, A, B, C, R(17))
    P(C, D, E, A, B, R(18))
    P(B, C, D, E, A, R(19))

#undef K
#undef F

#define F(x, y, z) (x ^ y ^ z)
#define K 0x6ED9EBA1

    P(A, B, C, D, E, R(20))
    P(E, A, B, C, D, R(21))
    P(D, E, A, B, C, R(22))
    P(C, D, E, A, B, R(23))
    P(B, C, D, E, A, R(24))
    P(A, B, C, D, E, R(25))
    P(E, A, B, C, D, R(26))
    P(D, E, A, B, C, R(27))
    P(C, D, E, A, B, R(28))
    P(B, C, D, E, A, R(29))
    P(A, B, C, D, E, R(30))
    P(E, A, B, C, D, R(31))
    P(D, E, A, B, C, R(32))
    P(C, D, E, A, B, R(33))
    P(B, C, D, E, A, R(34))
    P(A, B, C, D, E, R(35))
    P(E, A, B, C, D, R(36))
    P(D, E, A, B, C, R(37))
    P(C, D, E, A, B, R(38))
    P(B, C, D, E, A, R(39))

#undef K
#undef F

#define F(x, y, z) ((x & y) | (z & (x | y)))
#define K 0x8F1BBCDC

    P(A, B, C, D, E, R(40))
    P(E, A, B, C, D, R(41))
    P(D, E, A, B, C, R(42))
    P(C, D, E, A, B, R(43))
    P(B, C, D, E, A, R(44))
    P(A, B, C, D, E, R(45))
    P(E, A, B, C, D, R(46))
    P(D, E, A, B, C, R(47))
    P(C, D, E, A, B, R(48))
    P(B, C, D, E, A, R(49))
    P(A, B, C, D, E, R(50))
    P(E, A, B, C, D, R(51))
    P(D, E, A, B, C, R(52))
    P(C, D, E, A, B, R(53))
    P(B, C, D, E, A, R(54))
    P(A, B, C, D, E, R(55))
    P(E, A, B, C, D, R(56))
    P(D, E, A, B, C, R(57))
    P(C, D, E, A, B, R(58))
    P(B, C, D, E, A, R(59))

#undef K
#undef F

#define F(x, y, z) (x ^ y ^ z)
#define K 0xCA62C1D6

    P(A, B, C, D, E, R(60))
    P(E, A, B, C, D, R(61))
    P(D, E, A, B, C, R(62))
    P(C, D, E, A, B, R(63))
    P(B, C, D, E, A, R(64))
    P(A, B, C, D, E, R(65))
    P(E, A, B, C, D, R(66))
    P(D, E, A, B, C, R(67))
    P(C, D, E, A, B, R(68))
    P(B, C, D, E, A, R(69))
    P(A, B, C, D, E, R(70))
    P(E, A, B, C, D, R(71))
    P(D, E, A, B, C, R(72))
    P(C, D, E, A, B, R(73))
    P(B, C, D, E, A, R(74))
    P(A, B, C, D, E, R(75))
    P(E, A, B, C, D, R(76))
    P(D, E, A, B, C, R(77))
    P(C, D, E, A, B, R(78))
    P(B, C, D, E, A, R(79))

#undef K
#undef F

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
    ctx->state[4] += E;
}

/*
 * SHA-1 process init
 */
void crypto_sha1_init(sha1_ctx_t *ctx)
{
    memset(ctx, 0, sizeof(sha1_ctx_t));
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
}

/*
 * SHA-1 process buffer
 */
void crypto_sha1_update(sha1_ctx_t *ctx, const uint8_t *input, uint32_t ilen)
{
    uint32_t fill;
    uint32_t left;

    if (ilen == 0) {
        return;
    }

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (uint32_t)ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < (uint32_t)ilen) {
        ctx->total[1]++;
    }

    if (left && ilen >= fill) {
        memcpy((void *)(ctx->buffer + left), input, fill);
        local_sha1_process(ctx, ctx->buffer);
        input += fill;
        ilen -= fill;
        left = 0;
    }

    while (ilen >= 64) {
        local_sha1_process(ctx, input);
        input += 64;
        ilen -= 64;
    }

    if (ilen > 0) {
        memcpy((void *)(ctx->buffer + left), input, ilen);
    }
}

/*
 * SHA-1 final digest
 */
void crypto_sha1_final(sha1_ctx_t *ctx, uint8_t *digest)
{
    uint32_t      last, padn;
    uint32_t      high, low;
    uint8_t msglen[8];

    high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
    low  = (ctx->total[0] << 3);

    PUT_UINT32_BE(high, msglen, 0)
    PUT_UINT32_BE(low, msglen, 4)

    last = ctx->total[0] & 0x3F;
    padn = (last < 56) ? (56 - last) : (120 - last);

    crypto_sha1_update(ctx, sha1_padding, padn);
    crypto_sha1_update(ctx, msglen, 8);

    PUT_UINT32_BE(ctx->state[0], digest, 0)
    PUT_UINT32_BE(ctx->state[1], digest, 4)
    PUT_UINT32_BE(ctx->state[2], digest, 8)
    PUT_UINT32_BE(ctx->state[3], digest, 12)
    PUT_UINT32_BE(ctx->state[4], digest, 16)
}

void get_wchar(FILE *des, wchar_t *wstr){
    setlocale(LC_ALL, "");
    fwscanf(des, L"%[^\n]%*lc", wstr);
}

void crypto_SHA1(byte_ptr ans, byte_arr src){
    /**
     * 对于读入的宽字符有以下要求：
     * 1:需要在fwscanf宽字符前设置函数运行环境:setlocale(LC_ALL, "");
     *          以确保fwscanf函数读入的形式正确！
     * 2:fwscanf的格式字符串为:L"%[^\n]%*lc",即:
     *          fwscanf(stdin, L"%[^\n]%*lc", a);
     * 3:2022.6.1更新:使用本库中的get_wchar函数即可
     **/
    sha1_ctx_t ctx;
    crypto_sha1_init(&ctx);
    crypto_sha1_update(&ctx, src.arr, src.len);
    crypto_sha1_final(&ctx, ans->arr);
    ans->len = 20;
}

void crypto_HMAC_SHA1(byte_ptr ans, byte_arr key, byte_arr m){
    byte_arr ipad, opad, s0, s1, temp, temp1;
    for(; key.len < 64; key.len++)
        key.arr[key.len] = 0x00;
    for(ipad.len = 0; ipad.len < 64; ipad.len++)
        ipad.arr[ipad.len] = 0x36;
    for(opad.len = 0; opad.len < 64; opad.len++)
        opad.arr[opad.len] = 0x5c;
    for(s0.len = 0; s0.len < 64; s0.len++)
        s0.arr[s0.len] = key.arr[s0.len] ^ opad.arr[s0.len];
    for(s1.len = 0; s1.len < 64; s1.len++)
        s1.arr[s1.len] = key.arr[s1.len] ^ ipad.arr[s1.len];
    byte_cpy(&temp, s1, s1.len);
    byte_cat(&temp, m);
    crypto_SHA1(&temp1, temp);
    byte_cpy(&temp, s0, s0.len);
    byte_cat(&temp, temp1);
    crypto_SHA1(ans, temp);
}
///SHA-1 END

///SM3 START
/* SM3 Constants */
static uint32_t T[2] = {0x79CC4519, 0x7A879D8A};

/* ROTate Left (circular left shift) */
static uint32_t ROTL(uint32_t x, uint8_t shift)
{
    shift %= 32;
    return (x << shift) | (x >> (32 - shift));
}

static uint32_t FF(uint32_t x, uint32_t y, uint32_t z, uint32_t j)
{
    if (j<16) /* 0 <= j <= 15 */{
        return x ^ y ^ z;
    }
    else /* 16 <= j <= 63 */{
        return (x & y) | (x & z) | (y & z);
    }
}

static uint32_t GG(uint32_t x, uint32_t y, uint32_t z, uint32_t j){
    if (j<16) /* 0 <= j <= 15 */{
        return x ^ y ^ z;
    }
    else /* 16 <= j <= 63 */{
        return (x & y) | (~x & z);
    }
}

/* P0, Permutation 0 */
static uint32_t P0(uint32_t x){
    return x ^ ROTL(x, 9) ^ ROTL(x, 17);
}

/* P1, Permutation 1 */
static uint32_t P1(uint32_t x){
    return x ^ ROTL(x, 15) ^ ROTL(x, 23);
}

int SM3_Init(SM3_CTX *c){
    if (NULL == c){
        return ERR_INV_PARAM;
    }

    memset(c, 0, sizeof(SM3_CTX));

    /* Initial Value for SM3 */
    c->hash.a = 0x7380166f;
    c->hash.b = 0x4914b2b9;
    c->hash.c = 0x172442d7;
    c->hash.d = 0xda8a0600;
    c->hash.e = 0xa96f30bc;
    c->hash.f = 0x163138aa;
    c->hash.g = 0xe38dee4d;
    c->hash.h = 0xb0fb0e4e;

    return ERR_OK;
}

static int SM3_PrepareScheduleWord(const uint32_t *block, uint32_t *W, uint32_t *Wp){
    uint32_t j;

    if ((NULL == block) || (NULL == W) || (NULL == Wp)){
        return ERR_INV_PARAM;
    }

    /* Array W */
    for (j=0; j<(HASH_ROUND_NUM+4); j++){
        if (j<=15) /*  0 <= j <= 15 */
            W[j] = be32toh(block[j]);
        else	   /* 16 <= j <= 67 */
            W[j] = P1(W[j-16] ^ W[j-9] ^ ROTL(W[j-3],15)) ^ ROTL(W[j-13],7) ^ W[j-6];
    }

    /* Array W Prime */
    for (j=0; j<HASH_ROUND_NUM; j++){
        Wp[j] = W[j] ^ W[j+4];
    }

#if (DUMP_SCHED_DATA == 1)
    printf("          W1...W67:\n");
    for (j=0; j<(HASH_ROUND_NUM+4); j++)
    {
        if (j%8 == 0) /* line indent */
        {
            printf("          ");
        }

        printf("%08x ", W[j]);

        if (j%8 == 7)
        {
            printf("\n");
        }
        else if (j == (HASH_ROUND_NUM+4-1))
        {
            printf("\n"); /* last one */
        }
    }

    printf("          W'1...W'63:\n");
    for (j=0; j<HASH_ROUND_NUM; j++)
    {
        if (j%8 == 0) /* line indent */
        {
            printf("          ");
        }

        printf("%08x ", Wp[j]);

        if (j%8 == 7)
        {
            printf("\n");
        }
        else if (j == HASH_ROUND_NUM-1)
        {
            printf("\n"); /* last one */
        }
    }
#endif

    return ERR_OK;
}

static int SM3_ProcessBlock(SM3_CTX *ctx, const void *block){
    uint32_t j;
    uint32_t W[HASH_ROUND_NUM+4], Wp[HASH_ROUND_NUM];
    uint32_t SS1, SS2;
    uint32_t TT1, TT2;
    uint32_t A, B, C, D, E, F, G, H;

    if ((NULL == ctx) || (NULL == block))
    {
        return ERR_INV_PARAM;
    }

#if (DUMP_BLOCK_DATA == 1)
    DBG("---------------------------------------------------------\n");
    DBG("   BLOCK: %llu\n", ctx->total/HASH_BLOCK_SIZE);
    DBG("    DATA:\n");
    print_buffer(block, HASH_BLOCK_SIZE, "    ");
#endif

    /* prepare schedule word */
    SM3_PrepareScheduleWord(block, W, Wp);

    A = ctx->hash.a;
    B = ctx->hash.b;
    C = ctx->hash.c;
    D = ctx->hash.d;
    E = ctx->hash.e;
    F = ctx->hash.f;
    G = ctx->hash.g;
    H = ctx->hash.h;

#if (DUMP_BLOCK_HASH == 1)
    DBG("      IV: %08x %08x %08x %08x %08x %08x %08x %08x\n",
        ctx->hash.a, ctx->hash.b, ctx->hash.c, ctx->hash.d, ctx->hash.e, ctx->hash.f, ctx->hash.g, ctx->hash.h);
#endif

    for (j=0; j<HASH_ROUND_NUM; j++){
        SS1 = ROTL(ROTL(A, 12) + E + ROTL(T[j<16?0:1], j), 7);
        SS2 = SS1 ^ ROTL(A, 12);
        TT1 = FF(A, B, C, j) + D + SS2 + Wp[j];
        TT2 = GG(E, F, G, j) + H + SS1 + W[j];
        D = C;
        C = ROTL(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTL(F, 19);
        F = E;
        E = P0(TT2);

#if (DUMP_ROUND_DATA == 1)
        #if 1 /* Don't show temp variables: SS1/SS2/TT1/TT2/W/W' */
        DBG("      %02d: A=0x%08x, B=0x%08x, C=0x%08x, D=0x%08x, E=0x%08x, F=0x%08x, G=0x%08x, H=0x%08x\n", \
                j, A, B, C, D, E, F, G, H);
#else
        DBG("      %02d: SS1=0x%08x, SS2=0x%08x, TT1=0x%08x, TT2=0x%08x, W=0x%08x, Wp=0x%08x\n"\
            "         A=0x%08x,    B=0x%08x,   C=0x%08x,   D=0x%08x, E=0x%08x, F=0x%08x, G=0x%08x, H=0x%08x\n", \
                j, SS1, SS2, TT1, TT2, W[j], Wp[j], A, B, C, D, E, F, G, H);
#endif
#endif
    }

    ctx->hash.a ^= A;
    ctx->hash.b ^= B;
    ctx->hash.c ^= C;
    ctx->hash.d ^= D;
    ctx->hash.e ^= E;
    ctx->hash.f ^= F;
    ctx->hash.g ^= G;
    ctx->hash.h ^= H;

#if (DUMP_BLOCK_HASH == 1)
    DBG("    HASH: %08x %08x %08x %08x %08x %08x %08x %08x\n",
        ctx->hash.a, ctx->hash.b, ctx->hash.c, ctx->hash.d, ctx->hash.e, ctx->hash.f, ctx->hash.g, ctx->hash.h);
#endif

    return ERR_OK;
}


int SM3_Update(SM3_CTX *c, const void *data, size_t len){
    uint32_t copy_len = 0;

    if ((NULL == c) || (NULL == data))
    {
        return ERR_INV_PARAM;
    }

    /* has used data */
    if (c->last.used != 0)
    {
        /* less than 1 block in total, combine data */
        if (c->last.used + len < HASH_BLOCK_SIZE)
        {
            memcpy(&c->last.buf[c->last.used], data, len);
            c->last.used += len;

            return ERR_OK;
        }
        else /* more than 1 block */
        {
            /* process the block in context buffer */
            copy_len = HASH_BLOCK_SIZE - c->last.used;
            memcpy(&c->last.buf[c->last.used], data, copy_len);
            SM3_ProcessBlock(c, &c->last.buf);
            c->total += HASH_BLOCK_SIZE;

            data = (uint8_t *)data + copy_len;
            len -= copy_len;

            /* reset context buffer */
            memset(&c->last.buf[0], 0, HASH_BLOCK_SIZE);
            c->last.used = 0;
        }
    }

    /* less than 1 block, copy to context buffer */
    if (len < HASH_BLOCK_SIZE)
    {
        memcpy(&c->last.buf[c->last.used], data, len);
        c->last.used += len;

        return ERR_OK;
    }
    else
    {
        /* process data blocks */
        while (len >= HASH_BLOCK_SIZE)
        {
            SM3_ProcessBlock(c, data);
            c->total += HASH_BLOCK_SIZE;

            data = (uint8_t *)data + HASH_BLOCK_SIZE;
            len -= HASH_BLOCK_SIZE;
        }

        /* copy rest data to context buffer */
        memcpy(&c->last.buf[0], data, len);
        c->last.used = len;
    }

    return ERR_OK;
}

int SM3_Final(unsigned char *md, SM3_CTX *c){
    uint32_t *temp;
    //uint64_t *buf;

    if ((NULL == c) || (NULL == md))
    {
        return ERR_INV_PARAM;
    }

    /* Last block should be less thant HASH_BLOCK_SIZE - HASH_LEN_SIZE */
    if (c->last.used >= (HASH_BLOCK_SIZE - HASH_LEN_SIZE))
    {
        c->total += c->last.used;

        /* one more block */
        c->last.buf[c->last.used] = HASH_PADDING_PATTERN;
        c->last.used++;

        memset(&c->last.buf[c->last.used], 0, HASH_BLOCK_SIZE - c->last.used);
        SM3_ProcessBlock(c, &c->last.buf);

        memset(&c->last.buf[0], 0, HASH_BLOCK_SIZE - HASH_LEN_SIZE);
        c->last.used = 0;

        /* save length */
        //buf = (uint64_t *)&(c->last.buf[HASH_LEN_OFFSET]);
        //*buf = htobe64(c->total << 3);
        temp = (uint32_t *)&(c->last.buf[HASH_LEN_OFFSET]);
        temp[0] = htobe32((c->total << 3) >> 32 & 0xFFFFFFFF);
        temp[1] = htobe32((c->total << 3) & 0xFFFFFFFF);

        SM3_ProcessBlock(c, &c->last.buf);
    }
    else /* 0 <= last.used < HASH_BLOCK_SIZE - HASH_LEN_SIZE */
    {
        c->total += c->last.used;

        /* one more block */
        c->last.buf[c->last.used] = HASH_PADDING_PATTERN;
        c->last.used++;

        /* padding 0s */
        memset(&c->last.buf[c->last.used], 0, HASH_BLOCK_SIZE - HASH_LEN_SIZE - c->last.used);

        /* save length */
        //buf = (uint64_t *)&(c->last.buf[HASH_LEN_OFFSET]);
        //*buf = htobe64(c->total << 3);
        temp = (uint32_t *)&(c->last.buf[HASH_LEN_OFFSET]);
        temp[0] = htobe32((c->total << 3) >> 32 & 0xFFFFFFFF);
        temp[1] = htobe32((c->total << 3) & 0xFFFFFFFF);

        SM3_ProcessBlock(c, &c->last.buf);
    }

    temp = (uint32_t *)md;
    temp[0] = htobe32(c->hash.a);
    temp[1] = htobe32(c->hash.b);
    temp[2] = htobe32(c->hash.c);
    temp[3] = htobe32(c->hash.d);
    temp[4] = htobe32(c->hash.e);
    temp[5] = htobe32(c->hash.f);
    temp[6] = htobe32(c->hash.g);
    temp[7] = htobe32(c->hash.h);

    return ERR_OK;
}

byte *SM3(const unsigned char *d, size_t n, unsigned char *md){
    SM3_CTX c;
    uint64_t group, mod, cnt, cnt1;
    byte buf[LEN];
    SM3_Init(&c);
    ///计算消息能构成HASH_DIGEST_SIZE(32)字节的组数和剩余字节数
    group = n/HASH_DIGEST_SIZE;
    mod = n%HASH_DIGEST_SIZE;
    ///使用这些构成完整HASH_DIGEST_SIZE(32)字节的组和剩余字节构成的组更新SM3_CTX变量c
    for(cnt = 0; cnt < group; cnt++){
        for(cnt1 = 0; cnt1 < HASH_DIGEST_SIZE; cnt1++)
            buf[cnt1] = d[cnt*HASH_DIGEST_SIZE+cnt1];
        SM3_Update(&c, buf, HASH_DIGEST_SIZE);
    }
    for(cnt1 = 0; cnt1 < mod; cnt1++)
        buf[cnt1] = d[group*HASH_DIGEST_SIZE+cnt1];
    SM3_Update(&c, buf, mod);
    ///通过得到的已更新的SM3_CTX变量c计算摘要ans
    SM3_Final(md, &c);
    return md;
}
///SM3 END
