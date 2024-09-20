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
#include "../library_file/My_BigNum_GF_cal/My_BigNum_GF_cal.h"
#include "../library_file/RSA_Cipher/RSA_enc_dec.h"

int main(){
    mpz_t e1, e2, c1, c2, N, temp, ans;
    mpz_init(e1);
    mpz_init(e2);
    mpz_init(c1);
    mpz_init(c2);
    mpz_init(N);
    mpz_init(temp);
    mpz_init(ans);
    gmp_scanf("%Zd", &e1);
    gmp_scanf("%Zd", &e2);
    gmp_scanf("%Zd", &c1);
    gmp_scanf("%Zd", &c2);
    gmp_scanf("%Zd", &N);
    CM_Attack(&ans, e1, e2, c1, c2, N);
    gmp_printf("%Zd", ans);
    return 0;
}