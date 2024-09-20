//
// Created by 赵睿智 on 2022/6/4.
//

#ifndef CRYPTO_EXPERIMENT_DIGITAL_SIGNATURE_H
#define CRYPTO_EXPERIMENT_DIGITAL_SIGNATURE_H

#include <gmp.h>
#include <openssl/sha.h>
#include "../My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../conversion/lib_Convert.h"
#include "../ECC/ECC_enc_dec.h"
#include "../HashLib/Hash.h"

#ifndef HashDigest_Length
#define HashDigest_Length(hash_func, len)         \
    {                                             \
    if((hash_func) == SHA1) (*len) = 20;          \
    else if((hash_func) == SHA224) (*len) = 224/8;\
    else if((hash_func) == SHA256) (*len) = 256/8;\
    else if((hash_func) == SHA384) (*len) = 384/8;\
    else if((hash_func) == SHA512) (*len) = 512/8;\
    else if((hash_func) == SM3) (*len) = 32;\
    }
#endif

typedef struct ElGamal_Parm{
    mpz_t p;
    mpz_t g;
}elP, *elP_ptr;

typedef struct Schnorr_Parm{
    mpz_t p;
    mpz_t q;
    mpz_t a;
}scP, *scP_ptr;

typedef struct Signature_pair{
    mpz_t S1;
    mpz_t S2;
}sig, *sig_p;

void init_ElGamal_Parm(elP_ptr p);
void init_Schnorr_Parm(scP_ptr p);
void init_Sig(sig_p s);

void ElGamal_Sign(sig_p ans, byte_arr message, mpz_t secret_k, mpz_t random, elP parm,
                  unsigned char *(hash_func)(const unsigned char *, size_t, unsigned char *));
bool ElGamal_Vrfy(byte_arr message, mpz_t public_k, sig sig_pair, elP parm,
                  unsigned char *(hash_func)(const unsigned char *, size_t, unsigned char *));

void Schnorr_Sign(sig_p ans, byte_arr message, mpz_t secret_k, mpz_t random, scP parm,
                  unsigned char *(hash_func)(const unsigned char *, size_t, unsigned char *));
bool Schnorr_Vrfy(byte_arr message, mpz_t public_k, sig sig_pair, scP parm,
                  unsigned char *(hash_func)(const unsigned char *, size_t, unsigned char *));

void SM2_genZ(byte_ptr Z,
              wchar_t *ID_a, point publicK_a,point G, EC ec,
              byte *(hash_func)(const byte *, size_t, byte *));

void SM2_Sign(sig_p ans, byte_arr message,
              wchar_t *ID_a, mpz_t secretK_a, point publicK_a,
              mpz_t random, point G, EC ec,
              unsigned char *(hash_func)(const unsigned char *, size_t, unsigned char *));

bool SM2_vrfy(byte_arr message, sig sig_pair,
              wchar_t *ID_a, point publicK_a,
              point G, EC ec,
              byte *(hash_func)(const byte *, size_t, byte *));

#endif //CRYPTO_EXPERIMENT_DIGITAL_SIGNATURE_H
