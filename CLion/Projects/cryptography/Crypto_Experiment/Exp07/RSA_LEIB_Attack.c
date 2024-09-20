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
    int group_num, cnt;
    mpz_t e, r, p, src_r[LEN], src_p[LEN], ans;
    mpz_init(e);        mpz_init(ans);
    mpz_init(r);        mpz_init(p);
    for(cnt = 0; cnt < LEN; cnt++){
        mpz_init(src_r[cnt]);
        mpz_init(src_p[cnt]);
    }
    scanf("%d", &group_num);
    gmp_scanf("%Zd", &e);
    for(cnt = 0; cnt < group_num; cnt++){
        gmp_scanf("%Zd", &src_r[cnt]);
        gmp_scanf("%Zd", &src_p[cnt]);
    }
    LEIB_Attack(&ans, src_r, src_p, group_num);
    gmp_printf("%Zd", ans);
    return 0;
}