//
// Created by 赵睿智 on 2022/6/4.
//

#include "Digital_Signature.h"

void init_ElGamal_Parm(elP_ptr p){
    mpz_init(p->p);
    mpz_init(p->g);
}

void init_Schnorr_Parm(scP_ptr p){
    mpz_init(p->p);
    mpz_init(p->q);
    mpz_init(p->a);
}

void init_Sig(sig_p s){
    mpz_init(s->S1);
    mpz_init(s->S2);
}

void ElGamal_Sign(sig_p ans, byte_arr message, mpz_t secret_k, mpz_t random, elP parm,
                  byte *(hash_func)(const byte *, size_t, byte *)){
    init_Sig(ans);
    mpz_t inv_rand, p_1, m;
    mpz_init(inv_rand);
    mpz_init(p_1);
    mpz_sub_ui(p_1, parm.p, 1);
    mpz_invert(inv_rand, random, p_1);
    byte_arr hash_m;
    hash_func(message.arr, message.len, hash_m.arr);
    HashDigest_Length((hash_func), &(hash_m.len))
    byteStr2num(&m, hash_m);
    Fp(&ans->S1, parm.g, random, parm.p);
    mpz_mul(ans->S2, secret_k, ans->S1);
    mpz_neg(ans->S2, ans->S2);
    mpz_add(ans->S2, ans->S2, m);
    mpz_mul(ans->S2, ans->S2, inv_rand);
    mpz_mod(ans->S2, ans->S2, p_1);
    mpz_clear(inv_rand);
    mpz_clear(p_1);
    mpz_clear(m);
}

bool ElGamal_Vrfy(byte_arr message, mpz_t public_k, sig sig_pair, elP parm,
                  byte *(hash_func)(const byte *, size_t, byte *)){
    mpz_t V1, V2, temp1, temp2, m;
    mpz_init(V1);
    mpz_init(V2);
    mpz_init(temp1);
    mpz_init(temp2);
    byte_arr hash_m;
    hash_func(message.arr, message.len, hash_m.arr);
    HashDigest_Length((hash_func), &(hash_m.len))
    byteStr2num(&m, hash_m);
    Fp(&V1, parm.g, m, parm.p);
    Fp(&temp1, public_k, sig_pair.S1, parm.p);
    Fp(&temp2, sig_pair.S1, sig_pair.S2, parm.p);
    mpz_mul(V2, temp1, temp2);
    mpz_mod(V2, V2, parm.p);
    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(m);
    if(mpz_cmp(V1, V2) == 0){
        mpz_clear(V1);
        mpz_clear(V2);
        return true;
    }
    else{
        mpz_clear(V1);
        mpz_clear(V2);
        return false;
    }
}

void Schnorr_Sign(sig_p ans, byte_arr message, mpz_t secret_k, mpz_t random, scP parm,
                  byte *(hash_func)(const byte *, size_t, byte *)){
    init_Sig(ans);
    mpz_t x;
    mpz_init(x);
    Fp(&x, parm.a, random, parm.p);
    char *str_x = NULL;
    str_x = mpz_get_str(str_x, 10, x);
    for(int cnt = 0; cnt < strlen(str_x); cnt++)
        message.arr[message.len++] = str_x[cnt];
    byte_arr hash_m;
    hash_func(message.arr, message.len, hash_m.arr);
    HashDigest_Length((hash_func), &(hash_m.len))
    byteStr2num(&ans->S1, hash_m);
    mpz_mul(ans->S2, secret_k, ans->S1);
    mpz_add(ans->S2, ans->S2, random);
    mpz_mod(ans->S2, ans->S2, parm.q);
    mpz_clear(x);
    free(str_x);
}

bool Schnorr_Vrfy(byte_arr message, mpz_t public_k, sig sig_pair, scP parm,
                  byte *(hash_func)(const byte *, size_t, byte *)){
    mpz_t V1, V2, temp1, temp2;
    mpz_init_set(V1, sig_pair.S1);
    mpz_init(V2);
    mpz_init(temp1);
    mpz_init(temp2);
    Fp(&temp1, parm.a, sig_pair.S2, parm.p);
    Fp(&temp2, public_k, sig_pair.S1, parm.p);
    mpz_mul(temp1, temp1, temp2);
    mpz_mod(temp1, temp1, parm.p);
    char *str_x = NULL;
    str_x = mpz_get_str(str_x, 10, temp1);
    for(int cnt = 0; cnt < strlen(str_x); cnt++)
        message.arr[message.len++] = str_x[cnt];
    byte_arr hash_m;
    hash_func(message.arr, message.len, hash_m.arr);
    HashDigest_Length((hash_func), &(hash_m.len))
    byteStr2num(&V2, hash_m);
    mpz_clear(temp1);
    mpz_clear(temp2);
    if(mpz_cmp(V1, V2) == 0){
        mpz_clear(V1);
        mpz_clear(V2);
        return true;
    }
    else{
        mpz_clear(V1);
        mpz_clear(V2);
        return false;
    }
}

void SM2_genZ(byte_ptr Z,
              wchar_t *ID_a, point publicK_a, point G, EC ec,
              byte *(hash_func)(const byte *, size_t, byte *)){
    int secure_parm = 32;
    byte_arr bID_a, joint, ba, bb, bxg, byg, bxa, bya;
    WStr2ByteStr_utf8(&bID_a, ID_a);
    uint64_t len = bID_a.len*8;
    joint.len = 2;
    joint.arr[1] = len & 0xff;
    joint.arr[0] = len >> 8;
    byte_cat(&joint, bID_a);
    mpzNum2byteString(&ba, ec.a, secure_parm);          byte_cat(&joint, ba);
    mpzNum2byteString(&bb, ec.b, secure_parm);          byte_cat(&joint, bb);
    mpzNum2byteString(&bxg, G.x, secure_parm);          byte_cat(&joint, bxg);
    mpzNum2byteString(&byg, G.y, secure_parm);          byte_cat(&joint, byg);
    mpzNum2byteString(&bxa, publicK_a.x, secure_parm);  byte_cat(&joint, bxa);
    mpzNum2byteString(&bya, publicK_a.y, secure_parm);  byte_cat(&joint, bya);
    hash_func(joint.arr, joint.len, Z->arr);
    HashDigest_Length(hash_func, &Z->len)
}

void SM2_Sign(sig_p ans, byte_arr message,
              wchar_t *ID_a, mpz_t secretK_a, point publicK_a,
              mpz_t random, point G, EC ec,
              byte *(hash_func)(const byte *, size_t, byte *)){
    init_Sig(ans);
    byte_arr Za, be;
    point x;
    mpz_t e, inv, temp;
    mpz_init(e);
    mpz_init(inv);
    mpz_init(temp);
    SM2_genZ(&Za, ID_a, publicK_a, G, ec, SM3);
    byte_cat(&Za, message);
    hash_func(Za.arr, Za.len, be.arr);
    HashDigest_Length(hash_func, &be.len)
    byteStr2num(&e, be);
    sMul_point(&x, G, random, ec);
    mpz_add(ans->S1, x.x, e);
    mpz_mod(ans->S1, ans->S1, G.order);
    mpz_add_ui(inv, secretK_a, 1);
    mpz_invert(inv, inv, G.order);
    mpz_mul(temp, ans->S1, secretK_a);
    mpz_neg(temp, temp);
    mpz_add(temp, temp, random);
    mpz_mul(ans->S2, inv, temp);
    mpz_mod(ans->S2, ans->S2, G.order);
    mpz_clear(e);
    mpz_clear(inv);
    mpz_clear(temp);
    clear_point(&x);
}

bool SM2_vrfy(byte_arr message, sig sig_pair,
              wchar_t *ID_a, point publicK_a,
              point G, EC ec,
              byte *(hash_func)(const byte *, size_t, byte *)){
    byte_arr Za, be;
    SM2_genZ(&Za, ID_a, publicK_a, G, ec, SM3);
    byte_cat(&Za, message);
    hash_func(Za.arr, Za.len, be.arr);
    HashDigest_Length(hash_func, &be.len)
    mpz_t e, temp, R;
    mpz_init(e);
    mpz_init(temp);
    mpz_init(R);
    byteStr2num(&e, be);
    mpz_add(temp, sig_pair.S1, sig_pair.S2);
    mpz_mod(temp, temp, G.order);
    if(mpz_cmp_ui(temp, 0) == 0)
        return false;
    point t1, t2, t;
    init_point(&t1);
    init_point(&t2);
    init_point(&t);
    sMul_point(&t1, G, sig_pair.S2, ec);
    sMul_point(&t2, publicK_a, temp, ec);
    Add_point(&t, t1, t2, ec);
    mpz_add(R, e, t.x);
    mpz_mod(R, R, G.order);
    mpz_clear(e);
    mpz_clear(temp);
    clear_point(&t1);
    clear_point(&t2);
    clear_point(&t);
    if(mpz_cmp(R, sig_pair.S1) == 0) {
        mpz_clear(R);
        return true;
    }
    else{
        mpz_clear(R);
        return false;
    }
}