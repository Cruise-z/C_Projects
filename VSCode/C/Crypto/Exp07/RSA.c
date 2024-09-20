//
// Created by 赵睿智 on 2022/4/28.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include "../../.lib/My_BigNum_GF_cal/My_BigNum_GF_cal.h"

#define LEN 2048

int main(){
    mpz_t p, q, e, text, n;
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(text);
    mpz_init(n);
    gmp_scanf("%Zd", &p);
    gmp_scanf("%Zd", &q);
    gmp_scanf("%Zd", &e);
    gmp_scanf("%Zd", &text);
    mpz_mul(n, p, q);
    int op;
    scanf("%d", &op);
    if(op == 1){
        mpz_t c;
        mpz_init(c);
        Fp(&c, text, e, n);
        gmp_printf("%Zd", c);
    }else if(op == 0){
        mpz_t phi_n, temp, d, m;
        mpz_init(phi_n);
        mpz_init(temp);
        mpz_init(d);
        mpz_init(m);
        mpz_sub_ui(temp, p, 1);
        mpz_sub_ui(phi_n, q, 1);
        mpz_mul(phi_n, phi_n, temp);
        mpz_invert(d, e, phi_n);
        Fp(&m, text, d, n);
        gmp_printf("%Zd", m);
    }
    return 0;
}