//
// Created by 赵睿智 on 2022/5/13.
//

#include "ECC_enc_dec.h"

void init_point(p_ptr p){
    mpz_init(p->x);
    mpz_init(p->y);
    mpz_init(p->order);
}

void init_jpoint(jp_ptr p){
    mpz_init(p->x);
    mpz_init(p->y);
    mpz_init(p->z);
}

void init_set_point_ul(p_ptr p, uint64_t src_x, uint64_t src_y){
    mpz_init_set_ui(p->x, src_x);
    mpz_init_set_ui(p->y, src_y);
}

/*
 * 要牢记使用mpz_t的结构体直接用=赋值相当于指针传递，并没有生成副本的作用。所以写下了这个函数！！！
 */
void assign_point(p_ptr des, point src){
    mpz_init_set(des->x, src.x);
    mpz_init_set(des->y, src.y);
    mpz_init_set(des->order, src.order);
}

void assign_jpoint(jp_ptr des, jp src){
    mpz_init_set(des->x, src.x);
    mpz_init_set(des->y, src.y);
    mpz_init_set(des->z, src.z);
}

void clear_point(p_ptr des){
    mpz_clear(des->x);
    mpz_clear(des->y);
    mpz_clear(des->order);
}

void clear_jpoint(jp_ptr des){
    mpz_clear(des->x);
    mpz_clear(des->y);
    mpz_clear(des->z);
}

void init_EC(EC_ptr ptr){
    mpz_init(ptr->p);
    mpz_init(ptr->a);
    mpz_init(ptr->b);
}

void init_Cp(Cp_ptr ptr){
    init_point(&(ptr->C1));
    init_point(&(ptr->C2));
}

bool get_lambda(mpz_t *ans, point p, point q, EC C){
    mpz_init(*ans);
    if(mpz_cmp(p.x, q.x) == 0 && mpz_cmp(p.y, q.y) == 0){
        if(mpz_cmp_ui(p.y, 0) == 0)
            return false;
        mpz_t temp, temp1;
        mpz_init(temp);
        mpz_init(temp1);
        mpz_mul(temp, p.x, p.x);
        mpz_mul_ui(temp, temp, 3);
        mpz_add(temp, temp, C.a);
        mpz_mul_ui(temp1, p.y, 2);
        mpz_invert(temp1, temp1, C.p);
        mpz_mul(*ans, temp, temp1);
        mpz_mod(*ans, *ans, C.p);
        return true;
    }else{
        if(mpz_cmp(p.x, q.x) == 0)
            return false;
        mpz_t temp, temp1;
        mpz_init(temp);
        mpz_init(temp1);
        mpz_sub(temp, p.y, q.y);
        mpz_sub(temp1, p.x, q.x);
        mpz_invert(temp1, temp1, C.p);
        mpz_mul(*ans, temp, temp1);
        mpz_mod(*ans, *ans, C.p);
        return true;
    }
}

void Add_point(p_ptr ans, point p, point q, EC C){
    init_point(ans);
    point P, Q;
    assign_point(&P, p);
    assign_point(&Q, q);
    mpz_mod(P.x, P.x, C.p);      mpz_mod(P.y, P.y, C.p);
    mpz_mod(Q.x, Q.x, C.p);      mpz_mod(Q.y, Q.y, C.p);
    if(mpz_cmp_ui(P.x, 0) == 0 && mpz_cmp_ui(P.y, 0) == 0){
        assign_point(ans, Q);
        clear_point(&P);
        clear_point(&Q);
        return;
    }
    else if(mpz_cmp_ui(Q.x, 0) == 0 && mpz_cmp_ui(Q.y, 0) == 0){
        assign_point(ans, P);
        clear_point(&P);
        clear_point(&Q);
        return;
    }
    mpz_t lambda;
    bool j = get_lambda(&lambda, P, Q, C);
    if(j == false){
        mpz_set_ui(ans->x, 0);
        mpz_set_ui(ans->y, 0);
    }else{
        mpz_t temp;
        mpz_init(temp);
        mpz_mul(temp, lambda, lambda);
        mpz_sub(temp, temp, P.x);
        mpz_sub(temp, temp, Q.x);
        mpz_mod(ans->x, temp, C.p);
        mpz_sub(temp, P.x, ans->x);
        mpz_mul(temp, temp, lambda);
        mpz_sub(temp, temp, P.y);
        mpz_mod(ans->y, temp, C.p);
    }
    mpz_clear(lambda);
    clear_point(&P);
    clear_point(&Q);
}

void Sub_point(p_ptr ans, point p, point q, EC C){
    init_point(ans);
    point P, Q;
    assign_point(&P, p);
    assign_point(&Q, q);
    mpz_neg(Q.y, Q.y);
    mpz_mod(Q.y, Q.y, C.p);
    Add_point(ans, P, Q, C);
    clear_point(&P);
    clear_point(&Q);
}

//scalar mul:数乘
void sMul_point(p_ptr ans, point p, mpz_t k, EC C){
    point P, d;
    assign_point(&P, p);
    assign_point(&d, P);
    mpz_mod(k, k, C.p);
    mpz_t temp, mod;
    mpz_init_set(temp, k);
    mpz_init(mod);
    mpz_mod_ui(mod, temp, 2);
    mpz_div_ui(temp, temp, 2);
    mpz_cmp_ui(mod, 1) == 0? assign_point(ans, P): init_set_point_ul(ans, 0ull, 0ull);
    while(mpz_cmp_ui(temp, 0) != 0){
        Add_point(&d, d, d, C);
        mpz_mod_ui(mod, temp, 2);
        if(mpz_cmp_ui(mod, 1) == 0)
            Add_point(ans, *ans, d, C);
        mpz_div_ui(temp, temp, 2);
    }
    clear_point(&P);
    clear_point(&d);
    mpz_clear(temp);
    mpz_clear(mod);
}

//scalar div:数除
void sDiv_point(p_ptr ans, point p, mpz_t k, EC C){
    init_point(ans);
    mpz_t inv_k;
    mpz_init(inv_k);
    mpz_invert(inv_k, k, C.p);
    sMul_point(ans, p, inv_k, C);
    mpz_clear(inv_k);
}

void ECC_PK_enc(Cp_ptr ans, point A_Plaintext_point, mpz_t rand, point B_PK_point, EC C, point G){
    init_Cp(ans);
    sMul_point(&ans->C1, G, rand, C);
    sMul_point(&ans->C2, B_PK_point, rand, C);
    Add_point(&ans->C2, ans->C2, A_Plaintext_point, C);
}

void ECC_PK_fast_enc_pre(Cp_ptr pre, mpz_t rand, point B_PK_point, EC C, point G){
    init_Cp(pre);
    sMul_point(&pre->C1, G, rand, C);
    sMul_point(&pre->C2, B_PK_point, rand, C);
}

void ECC_PK_fast_enc(Cp_ptr ans, point A_Plaintext_point, Cp pre, EC C){
    assign_point(&ans->C1, pre.C1);
    Add_point(&ans->C2, pre.C2, A_Plaintext_point, C);
}

void ECC_PK_dec(p_ptr ans, Cp ciphertext_point, mpz_t B_SK, EC C){
    init_point(ans);
    point O;
    init_set_point_ul(&O, 0ull, 0ull);
    sMul_point(ans, ciphertext_point.C1, B_SK, C);
    Sub_point(ans, O, *ans, C);
    Add_point(ans, *ans, ciphertext_point.C2, C);
    clear_point(&O);
}

void ECC_Pk_fast_dec_pre(p_ptr pre, point cp1, mpz_t B_SK, EC C){
    point O;
    init_set_point_ul(&O, 0ull, 0ull);
    sMul_point(pre, cp1, B_SK, C);
    Sub_point(pre, O, *pre, C);
    clear_point(&O);
}

void ECC_Pk_fast_dec(p_ptr ans, Cp ciphertext_point, point pre, EC C){
    Add_point(ans, pre, ciphertext_point.C2, C);
}

int KDF_SHA256(bit_ptr K, bit_arr Z, uint64_t k_len){
    if(k_len >= ((1ull<<32)-1)*256)
        if(k_len > arrLEN)
            return -1;
    uint64_t ct, num;
    num = k_len/256+1;
    char hex_ct[9];
    byte_arr byte_ct, byte_Z, Ha, temp, byte_K;
    bit2byte(&byte_Z, Z, 0);
    byte_K.len = 0;
    for(ct = 1; ct <= num; ct++){
        sprintf(hex_ct, "%llx", ct);
        str2byteString(&byte_ct, hex_ct, 4);
        byte_cpy(&temp, byte_Z, byte_Z.len);
        byte_cat(&temp, byte_ct);
        SHA256(temp.arr, temp.len, Ha.arr);
        Ha.len = 32;
        byte_cat(&byte_K, Ha);
    }
    byte2bitString(K, byte_K, byte_K.len*8);
    K->len = k_len;
    return 0;
}

void ECC_encode(mpz_t *re, byte_arr M, mpz_t k, uint64_t Par, point G, point Pb, EC C){
    mpz_init(*re);
    point C1, C2;
    init_point(&C1);
    init_point(&C2);
    sMul_point(&C1, G, k, C);
    sMul_point(&C2, Pb, k, C);
    char *s_x1 = NULL, *s_y1 = NULL, *s_x2 = NULL, *s_y2 = NULL;
    s_x1 = mpz_get_str(s_x1, 2, C1.x);
    s_y1 = mpz_get_str(s_y1, 2, C1.y);
    s_x2 = mpz_get_str(s_x2, 2, C2.x);
    s_y2 = mpz_get_str(s_y2, 2, C2.y);
    bit_arr c1, b_x1, b_y1, b_x2, b_y2, xy2, bit_M, t, c2, xMy2, c3, bit_re;
    byte2bitString(&bit_M, M, M.len*8);
    str2bitString(&b_x1, s_x1, Par);
    str2bitString(&b_y1, s_y1, Par);
    str2bitString(&b_x2, s_x2, Par);
    str2bitString(&b_y2, s_y2, Par);
    byte_arr tag, byte_xMy2, byte_c3;
    tag.arr[0] = 0x04;
    tag.len = 1;
    byte2bitString(&c1, tag, 8);
    bit_cat(&c1, b_x1);
    bit_cat(&c1, b_y1);
    bit_copy(&xy2, b_x2, 0, b_x2.len);
    bit_cat(&xy2, b_y2);
    uint64_t kLen = bit_M.len;
    KDF_SHA256(&t, xy2, kLen);
    for(c2.len = 0; c2.len < kLen; c2.len++)
        c2.arr[c2.len] = bit_M.arr[c2.len] ^ t.arr[c2.len];
    bit_copy(&xMy2, b_x2, 0, b_x2.len);
    bit_cat(&xMy2, bit_M);
    bit_cat(&xMy2, b_y2);
    bit2byte(&byte_xMy2, xMy2, 0);
    SHA256(byte_xMy2.arr, byte_xMy2.len, byte_c3.arr);
    byte_c3.len = 32;
    byte2bitString(&c3, byte_c3, byte_c3.len*8);
    bit_copy(&bit_re, c1, 0, c1.len);
    bit_cat(&bit_re, c2);
    bit_cat(&bit_re, c3);
    bitStr2num(re, bit_re);
}

void ECC_decode(byte_ptr re, byte_arr M, uint64_t Par, point G, mpz_t Db, EC C){
    char *s_x2 = NULL, *s_y2 = NULL;
    bit_arr m, c1, c2, c3, c1x, c1y, b_x2, b_y2, xy2, t, bit_re;
    byte_arr tag;
    point C1, C2, Calibration;
    init_point(&C1);
    init_point(&C2);
    byte2bitString(&m, M, 0);
    uint64_t kLen = m.len-3-Par*2-32*8;
    bit_copy(&c1, m, 3, Par*2);
    bit_copy(&c2, m, 3+Par*2, kLen);
    bit_copy(&c3, m, m.len-32*8, 32*8);
    bit_copy(&c1x, c1, 0, Par);
    bit_copy(&c1y, c1, Par, Par);
    bitStr2num(&C1.x, c1x);
    bitStr2num(&C1.y, c1y);
    //校准
    sMul_point(&C2, C1, Db, C);
    s_x2 = mpz_get_str(s_x2, 2, C2.x);
    s_y2 = mpz_get_str(s_y2, 2, C2.y);
    str2bitString(&b_x2, s_x2, Par);
    str2bitString(&b_y2, s_y2, Par);
    bit_copy(&xy2, b_x2, 0, b_x2.len);
    bit_cat(&xy2, b_y2);
    KDF_SHA256(&t, xy2, kLen);
    //检验
    for(bit_re.len = 0; bit_re.len < kLen; bit_re.len++)
        bit_re.arr[bit_re.len] = c2.arr[bit_re.len] ^ t.arr[bit_re.len];
    bit2byte(re, bit_re, 0);
}

void convert_jp2p(p_ptr des, jp src, EC c){
    init_point(des);
    mpz_t z2, z3, inv_z2, inv_z3;
    mpz_init(z2);
    mpz_init(z3);
    mpz_init(inv_z2);
    mpz_init(inv_z3);
    mpz_mul(z2, src.z, src.z);
    mpz_mul(z3, z2, src.z);
    mpz_invert(inv_z2, z2, c.p);
    mpz_invert(inv_z3, z3, c.p);
    mpz_mul(des->x, src.x, inv_z2);
    mpz_mod(des->x, des->x, c.p);
    mpz_mul(des->y, src.y, inv_z3);
    mpz_mod(des->y, des->y, c.p);
    mpz_clear(z2);
    mpz_clear(z3);
    mpz_clear(inv_z2);
    mpz_clear(inv_z3);
}

void convert_p2jp(jp_ptr des, point src, EC c){
    init_jpoint(des);
    mpz_init_set(des->x, src.x);
    mpz_mod(des->x, des->x, c.p);
    mpz_init_set(des->y, src.y);
    mpz_mod(des->y, des->y, c.p);
    mpz_init_set_ui(des->z, 1);
}

void mmadd_2007_bl(jp_ptr ans, jp p, jp q, EC c){
    jp P, Q;
    assign_jpoint(&P, p);
    assign_jpoint(&Q, q);
    init_jpoint(ans);
    if(mpz_cmp_ui(P.x, 1)==0&&mpz_cmp_ui(P.y, 1)==0&&mpz_cmp_ui(P.z, 0)==0)
        assign_jpoint(ans, Q);
    else if(mpz_cmp_ui(Q.x, 1)==0&&mpz_cmp_ui(Q.y, 1)==0&&mpz_cmp_ui(Q.z, 0)==0)
        assign_jpoint(ans, P);
    else{
        mpz_t H, HH, I, J, R, V, temp;
        mpz_init(H);
        mpz_init(HH);
        mpz_init(I);
        mpz_init(J);
        mpz_init(R);
        mpz_init(V);
        mpz_init(temp);
        mpz_sub(H, Q.x, P.x);
        mpz_mul(HH, H, H);
        mpz_mul_ui(I, HH, 4);
        mpz_mul(J, H, I);
        mpz_sub(R, Q.y, P.y);
        mpz_mul_ui(R, R, 2);
        mpz_mul(V, P.x, I);
        //计算x
        mpz_mul(ans->x, R, R);
        mpz_sub(ans->x, ans->x, J);
        mpz_mul_ui(temp, V, 2);
        mpz_sub(ans->x, ans->x, temp);
        mpz_mod(ans->x, ans->x, c.p);
        //计算y
        mpz_mul(temp, P.y, J);
        mpz_mul_ui(temp, temp, 2);
        mpz_sub(ans->y, V, ans->x);
        mpz_mul(ans->y, ans->y, R);
        mpz_sub(ans->y, ans->y, temp);
        mpz_mod(ans->y, ans->y, c.p);
        //计算z
        mpz_mul_ui(ans->z, H, 2);
        mpz_mod(ans->z, ans->z, c.p);
        //清理空间
        mpz_clear(H);
        mpz_clear(HH);
        mpz_clear(I);
        mpz_clear(J);
        mpz_clear(R);
        mpz_clear(V);
        mpz_clear(temp);
    }
    clear_jpoint(&P);
    clear_jpoint(&Q);
}

void add_2007_bl(jp_ptr ans, jp p, jp q, EC c){
    jp P, Q;
    assign_jpoint(&P, p);
    assign_jpoint(&Q, q);
    init_jpoint(ans);
    if(mpz_cmp_ui(P.x, 1)==0&&mpz_cmp_ui(P.y, 1)==0&&mpz_cmp_ui(P.z, 0)==0)
        assign_jpoint(ans, Q);
    else if(mpz_cmp_ui(Q.x, 1)==0&&mpz_cmp_ui(Q.y, 1)==0&&mpz_cmp_ui(Q.z, 0)==0)
        assign_jpoint(ans, P);
    else{
        mpz_t Z1Z1, Z2Z2, U1, U2, S1, S2, H, I, J, R, V, temp;
        mpz_init(Z1Z1);
        mpz_init(Z2Z2);
        mpz_init(U1);
        mpz_init(U2);
        mpz_init(S1);
        mpz_init(S2);
        mpz_init(H);
        mpz_init(I);
        mpz_init(J);
        mpz_init(R);
        mpz_init(V);
        mpz_init(temp);
        mpz_mul(Z1Z1, P.z, P.z);
        mpz_mul(Z2Z2, Q.z, Q.z);
        mpz_mul(U1, P.x, Z2Z2);
        mpz_mul(U2, Q.x, Z1Z1);
        mpz_mul(S1, P.y, Q.z);
        mpz_mul(S1, S1, Z2Z2);
        mpz_mul(S2, Q.y, P.z);
        mpz_mul(S2, S2, Z1Z1);
        mpz_sub(H, U2, U1);
        mpz_mul_ui(I, H, 2);
        mpz_pow_ui(I, I, 2);
        mpz_mul(J, H, I);
        mpz_sub(R, S2, S1);
        mpz_mul_ui(R, R, 2);
        mpz_mul(V, U1, I);
        //计算x
        mpz_pow_ui(ans->x, R, 2);
        mpz_sub(ans->x, ans->x, J);
        mpz_mul_ui(temp, V, 2);
        mpz_sub(ans->x, ans->x, temp);
        mpz_mod(ans->x, ans->x, c.p);
        //计算y
        mpz_mul(temp, S1, J);
        mpz_mul_ui(temp, temp, 2);
        mpz_sub(ans->y, V, ans->x);
        mpz_mul(ans->y, ans->y, R);
        mpz_sub(ans->y, ans->y, temp);
        mpz_mod(ans->y, ans->y, c.p);
        //计算z
        mpz_add(ans->z, P.z, Q.z);
        mpz_pow_ui(ans->z, ans->z, 2);
        mpz_sub(ans->z, ans->z, Z1Z1);
        mpz_sub(ans->z, ans->z, Z2Z2);
        mpz_mul(ans->z, ans->z, H);
        mpz_mod(ans->z, ans->z, c.p);
        //清理空间
        mpz_clear(Z1Z1);
        mpz_clear(Z2Z2);
        mpz_clear(U1);
        mpz_clear(U2);
        mpz_clear(S1);
        mpz_clear(S2);
        mpz_clear(H);
        mpz_clear(I);
        mpz_clear(J);
        mpz_clear(R);
        mpz_clear(V);
        mpz_clear(temp);
    }
    clear_jpoint(&P);
    clear_jpoint(&Q);
}

void mdbl_2007_bl(jp_ptr ans, jp p, EC c){
    jp P;
    assign_jpoint(&P, p);
    init_jpoint(ans);
    if(mpz_cmp_ui(P.x, 1)==0&&mpz_cmp_ui(P.y, 1)==0&&mpz_cmp_ui(P.z, 0)==0)
        assign_jpoint(ans, P);
    else{
        mpz_t XX, YY, YYYY, S, M, T, temp;
        mpz_init(XX);
        mpz_init(YY);
        mpz_init(YYYY);
        mpz_init(S);
        mpz_init(M);
        mpz_init(T);
        mpz_init(temp);
        mpz_mul(XX, p.x, p.x);
        mpz_mul(YY, p.y, p.y);
        mpz_mul(YYYY, YY, YY);
        mpz_add(S, p.x, YY);
        mpz_mul(S, S, S);
        mpz_sub(S, S, XX);
        mpz_sub(S, S, YYYY);
        mpz_mul_ui(S, S, 2);
        mpz_mul_ui(M, XX, 3);
        mpz_add(M, M, c.a);
        mpz_mul(T, M, M);
        mpz_mul_ui(temp, S, 2);
        mpz_sub(T, T , temp);
        //计算x
        mpz_set(ans->x, T);
        mpz_mod(ans->x, ans->x, c.p);
        //计算y
        mpz_sub(ans->y, S, T);
        mpz_mul(ans->y, ans->y, M);
        mpz_mul_ui(temp, YYYY, 8);
        mpz_sub(ans->y, ans->y, temp);
        mpz_mod(ans->y, ans->y, c.p);
        //计算z
        mpz_mul_ui(ans->z, p.y, 2);
        mpz_mod(ans->z, ans->z, c.p);
        //清理空间
        mpz_clear(XX);
        mpz_clear(YY);
        mpz_clear(YYYY);
        mpz_clear(S);
        mpz_clear(M);
        mpz_clear(T);
        mpz_clear(temp);
    }
    clear_jpoint(&P);
}

void dbl_2007_bl(jp_ptr ans, jp p, EC c){
    jp P;
    assign_jpoint(&P, p);
    init_jpoint(ans);
    if(mpz_cmp_ui(P.x, 1)==0&&mpz_cmp_ui(P.y, 1)==0&&mpz_cmp_ui(P.z, 0)==0)
        assign_jpoint(ans, P);
    else{
        mpz_t XX, YY, YYYY, ZZ, S, M, T, temp;
        mpz_init(XX);
        mpz_init(YY);
        mpz_init(YYYY);
        mpz_init(ZZ);
        mpz_init(S);
        mpz_init(M);
        mpz_init(T);
        mpz_init(temp);
        mpz_mul(XX, p.x, p.x);
        mpz_mul(YY, p.y, p.y);
        mpz_mul(YYYY, YY, YY);
        mpz_mul(ZZ, p.z, p.z);
        mpz_add(S, p.x, YY);
        mpz_mul(S, S, S);
        mpz_sub(S, S, XX);
        mpz_sub(S, S, YYYY);
        mpz_mul_ui(S, S, 2);
        mpz_mul_ui(M, XX, 3);
        mpz_mul(temp, ZZ, ZZ);
        mpz_mul(temp, temp, c.a);
        mpz_add(M, M, temp);
        mpz_mul(T, M, M);
        mpz_mul_ui(temp, S, 2);
        mpz_sub(T, T , temp);
        //计算x
        mpz_set(ans->x, T);
        mpz_mod(ans->x, ans->x, c.p);
        //计算y
        mpz_sub(ans->y, S, T);
        mpz_mul(ans->y, ans->y, M);
        mpz_mul_ui(temp, YYYY, 8);
        mpz_sub(ans->y, ans->y, temp);
        mpz_mod(ans->y, ans->y, c.p);
        //计算z
        mpz_add(ans->z, p.y, p.z);
        mpz_pow_ui(ans->z, ans->z, 2);
        mpz_sub(ans->z, ans->z, YY);
        mpz_sub(ans->z, ans->z, ZZ);
        mpz_mod(ans->z, ans->z, c.p);
        //清理空间
        mpz_clear(XX);
        mpz_clear(YY);
        mpz_clear(YYYY);
        mpz_clear(S);
        mpz_clear(M);
        mpz_clear(T);
        mpz_clear(temp);
    }
    clear_jpoint(&P);
}

void sub_jpoint(jp_ptr ans, jp p, jp q, EC c){
    jp Q;
    init_jpoint(&Q);
    mpz_set(Q.x, q.x);
    mpz_sub(Q.y, c.p, q.y);
    mpz_set(Q.z, q.z);
    mmadd_2007_bl(ans, p, Q, c);
}

void slid_window_smul1(p_ptr ans, point P, mpz_t k, EC c){
    jp p[16], q;
    mpz_init_set_ui(q.x, 1);
    mpz_init_set_ui(q.y, 1);
    mpz_init_set_ui(q.z, 0);
    char *bit_k = NULL;
    bit_k = mpz_get_str(bit_k, 2, k);
    int l = (int)strlen(bit_k), i, j = l-1, t, h;
    convert_p2jp(&p[1], P, c);
    mdbl_2007_bl(&p[2], p[1], c);
    for(i = 1; i <= 7; i++)
        add_2007_bl(&p[2 * i + 1], p[2 * i - 1], p[2], c);
    while(j >= 0){
        h = 0;
        if(bit_k[l-1-j] == '0'){
            dbl_2007_bl(&q, q, c);
            j--;
        }else{
            j-3>0?(t = j-3):(t = 0);
            while(bit_k[l-1-t]!='1')
                t++;
            for(i = 0; i <= j-t; i++)
                h += (bit_k[l-1-t-i]-'0') * (1<<i);
            for(int cnt = 0; cnt < j-t+1; cnt++)
                dbl_2007_bl(&q, q, c);
            add_2007_bl(&q, q, p[h], c);
            j = t-1;
        }
    }
    convert_jp2p(ans, q, c);
    clear_jpoint(&p[1]);
    clear_jpoint(&p[2]);
    for(int cnt = 1; cnt < 8; cnt++)
        clear_jpoint(&p[2*cnt+1]);
    clear_jpoint(&q);
}