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

int main(){
    char *mod_str = "11b";
    char *s1 = malloc(sizeof(char)*LEN);
    mpz_gcd_re re;
    scanf("%s", s1);
    re = GF_2_hex_exEuclid(s1, mod_str, mod_str);
    puts(re.x_str);
    return 0;
}