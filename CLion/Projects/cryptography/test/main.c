#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct point_inf{
    mpz_t x;
    mpz_t y;
}point, *p_ptr;

typedef struct EC_inf{
    mpz_t p; //大素数
    mpz_t a;
    mpz_t b;
}EC, *EC_ptr;

typedef struct Cipher_point{
    point C1;
    point C2;
}Cp, *Cp_ptr;

void init_point(p_ptr p){
    mpz_init(p->x);
    mpz_init(p->y);
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
}

void clear_point(p_ptr des){
    mpz_clear(des->x);
    mpz_clear(des->y);
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

int main(){
    point p, q, ans;
    EC C;
    mpz_t k;
    mpz_init(k);
    init_EC(&C);
    init_point(&p);
    init_point(&q);
    gmp_scanf("%Zd%Zd%Zd", &C.p, &C.a, &C.b);
    gmp_scanf("%Zd%Zd", &p.x, &p.y);
    gmp_scanf("%Zd%Zd", &q.x, &q.y);
    gmp_scanf("%Zd", &k);
    Add_point(&ans, p, q, C);
    gmp_printf("%Zd %Zd\n", ans.x, ans.y);
    Sub_point(&ans, p, q, C);
    gmp_printf("%Zd %Zd\n", ans.x, ans.y);
    sMul_point(&ans, p, k, C);
    gmp_printf("%Zd %Zd\n", ans.x, ans.y);
    sDiv_point(&ans, q, k, C);
    gmp_printf("%Zd %Zd", ans.x, ans.y);
    return 0;
}
