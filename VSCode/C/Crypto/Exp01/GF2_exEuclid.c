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
    char *s1 = malloc(sizeof(char) * LEN), *s2 = malloc(sizeof(char) * LEN);
    scanf("%s%s", s1, s2);
    mpz_gcd_re re;
    re = GF_2_hex_exEuclid(s1, s2, mod_str);
    printf("%s %s %s", re.x_str, re.y_str, re.gcd_str);
    return 0;
}