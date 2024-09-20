//
// Created by 赵睿智 on 2022/3/16.
//

#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../../.lib/My_BigNum_GF_cal/My_BigNum_GF_cal.h"

#define LEN 2048


int main(int argc, char **argv) {
    char s1[LEN], s2[LEN], s3[LEN], anstr[LEN];
    scanf("%s%s%s", s1,s2,s3);
    mpz_t b,p,m,ans;
    mpz_init_set_str(b, s1, 10);
    mpz_init_set_str(p, s2, 10);
    mpz_init_set_str(m, s3, 10);
    Fp(&ans, b, p, m);
    mpz_get_str(anstr, 10, ans);
    puts(anstr);
    return 0;
}

