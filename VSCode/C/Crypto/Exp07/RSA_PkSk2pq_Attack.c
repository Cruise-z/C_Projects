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
#include "../../.lib/RSA_Cipher/RSA_enc_dec.h"

int main(){
    mpz_t e, d, N;
    mpz_init(e);
    mpz_init(d);
    mpz_init(N);
    gmp_scanf("%Zd%Zd%Zd", &e, &d, &N);
    mpz_t p, q;
    PkSk2pq_Attack(&p, &q, e, d, N);
    gmp_printf("%Zd\n", q);
    gmp_printf("%Zd", p);
}