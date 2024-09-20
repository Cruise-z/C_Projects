//
// Created by 赵睿智 on 2022/4/30.
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
    mpz_t e, N;
    mpz_t p, q, d;
    mpz_init(e);
    mpz_init(N);
    gmp_scanf("%Zd\n%Zd", &e, &N);
    Wiener_Attack(&p, &q, &d, e, N);
    gmp_printf("%Zd\n%Zd\n%Zd", p, q, d);
    return 0;
}